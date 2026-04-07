#include "parsing.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <variant>

#include <string>
#include <regex>

#include <cstdlib>

#include "material.h"

enum class mode {
    sphere,
    camera
};


void trim(std::string& s) {
    std::regex trim_regex("^\\s+|\\s+$");
    s = std::regex_replace(s, trim_regex, "");
}

void error_and_exit(std::string line, int line_no) {
    std::clog << "Line " << line_no << ": '" << line << "'\n";
    std::exit(1);
}


class parameter {
    public:
    std::string name;
    std::vector<std::variant<double, std::string>> args;

    // for error messages
    std::string line;
    int line_no;

    parameter(std::string line, int line_no) : line(line), line_no(line_no) {
        std::istringstream iss(line);

        std::string substr;
        iss >> name;

        while (iss >> substr) {
            try {
                args.push_back(std::stod(substr));
            } catch (const std::invalid_argument&) {
                args.push_back(substr);
            }
        }
    }

    // expected args should contain zero or more of 'd' or 's', representing double or string,
    // or + at the end to indicate that all following arguments should be ignored (e.g. in variadic parameters)
    void check_args(std::string expected_args) const {
        if (expected_args.back() == '+') { // arguments at the end ignored
            if (args.size() <= expected_args.size() - 1) {
                std::clog << "'" << name << "' expected more than " << expected_args << " arguments but got " << args.size() << '\n';
                error_and_exit(line, line_no);
            }
        } else if (args.size() != expected_args.size()) { // fixed number of arguments
            std::clog << "'" << name << "' expected" << expected_args << " arguments but got " << args.size() << '\n';
            error_and_exit(line, line_no);

        }

        for (int i = 0; i < expected_args.size(); ++i) {
            if (std::holds_alternative<double>(args[i]) && expected_args[i] == 's') {
                std::clog << "'" << name << "' expected a string for argument " << i+1 << "but got double: '" << std::get<double>(args[i]) << "'\n";
                error_and_exit(line, line_no);

            } else if (std::holds_alternative<std::string>(args[i]) && expected_args[i] == 'd') {
                std::clog << "'" << name << "' expected a double for argument " << i+1 << "but got string: '" << std::get<std::string>(args[i]) << "'\n";
                error_and_exit(line, line_no);
            }
        }
    }

    double as_double() const {
        check_args("d");
        return std::get<double>(args[0]);
    }

    std::string as_string() const {
        check_args("s");
        return std::get<std::string>(args[0]);
    }

    vec3 as_vec3() const {
        vec3 vec;
        check_args("ddd");

        for (int i = 0; i < 3; ++i) {
            vec[i] = std::get<double>(args[i]);
        }

        return vec;
    }

    point3 as_point3() const {
        return as_vec3(); // note that vec3 and point3 are the same class under the hood
    }

    // note that this function doesn't yet call check_args() (should probably change that in the future)
    color as_color(int start) const {
        color col;

        for (int i = 0; i < 3; ++i) {
            double intensity = std::get<double>(args[i + start]);
            if (intensity < 0 || intensity > 255) {
                std::cout << "Color intensity must be betwen 0 and 255 (inclusive), but was '" << intensity << "'\n";
                error_and_exit(line, line_no);
            }

            col[i] = intensity / 255;
        }

        return col;
    }

    color as_color() const {
        return as_color(0);
    }
};


mode parse_label(parameter& param) {
    if (!param.args.empty()) {
        std::clog << "Label '" << param.name << "' given arguments, when none were expected \n";
        error_and_exit(param.line, param.line_no);
    }

    if (param.name == "sphere:") {
        return mode::sphere;
    } else {
        std::clog << "Unknown label '" << param.name << "'\n";
        error_and_exit(param.line, param.line_no);
    }
}

void evaluate_camera_params(std::vector<parameter>& params, camera_params& c_params) {
    // default values
    int width = 640;
    int height = 360;

    point3 lookfrom(0, 0, 0);
    point3 lookat(0, 0, -1);
    vec3 vup(0, 1, 0);

    for (auto& param : params) {
        if (param.name == "image-size") {
            param.check_args("dd");

            width = std::get<double>(param.args[0]);
            height = std::get<double>(param.args[1]);
        } else if (param.name == "look-from") {
            lookfrom = param.as_point3();
        } else if (param.name == "look-at") {
            lookat = param.as_point3();
        } else if (param.name == "up-direction") {
            vup = param.as_vec3();
        } else if (param.name == "vertical-fov") {
            c_params.vfov = param.as_double();
        } else if (param.name == "samples-per-pixel") {
            c_params.samples_per_pixel = param.as_double();
        } else if (param.name == "max-depth") {
            c_params.max_depth == param.as_double();
        } else {
            std::clog << "Unknown camera parameter name '" << param.name << "'\n";
            error_and_exit(param.line, param.line_no);
        }
    }

    c_params.set_focus(lookfrom, lookat, vup);
    c_params.set_size(width, height);
}

void evaluate_sphere_params(std::vector<parameter>& params, object_group& objects) {
    point3 position(0, 0, 0);
    double size = 0;

    std::shared_ptr<material> mat = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));

    for (auto& param : params) {
        if (param.name == "position") {
            position = param.as_point3();

        } else if (param.name == "size") {
            size = param.as_double();

        } else if (param.name == "material") {
            param.check_args("s+");

            std::string material_name = std::get<std::string>(param.args[0]);
            if (material_name == "matt") {
                param.check_args("sddd");

                color col = param.as_color(1);
                mat = std::make_shared<lambertian>(col);

            } else if (material_name == "shiny") {
                param.check_args("sddd");

                color col = param.as_color(1);
                mat = std::make_shared<metal>(col, 0);

            } else if (material_name == "transparent") {
                param.check_args("sd");
                double refractive_index = std::get<double>(param.args[1]);
                mat = std::make_shared<dielectric>(refractive_index);
            }
        } else {
            std::clog << "Unknown sphere parameter name '" << param.name << "'\n";
            error_and_exit(param.line, param.line_no);
        }
    }

    auto s = std::make_shared<sphere>(position, size, mat);
    objects.add(s);
}

void evaluate_by_mode(mode current_mode, std::vector<parameter>& params, camera_params& c_params, object_group& objects) {
    switch (current_mode) {
        case mode::sphere:
            evaluate_sphere_params(params, objects);
            break;

        case mode::camera:
            evaluate_camera_params(params, c_params);
            break;
    }
}

void parse_file(std::string filename, camera_params& c_params, object_group& objects) {
    std::ifstream file(filename);
    std::string line;

    std::vector<parameter> params;

    mode current_mode = mode::camera;

    int line_no = 0;
    while (getline(file, line)) {
        ++line_no;

        trim(line);
        if (line.empty() || line[0] == '#') continue; // ignore blank lines and comments

        parameter param(line, line_no);

        if (param.name.back() == ':') { // check for labels
            // evaluate all parameters from the previous label
            evaluate_by_mode(current_mode, params, c_params, objects);
            params.clear();

            // evaluate the new label
            current_mode = parse_label(param);

        } else {
            params.push_back(param);
        }
    }

    // evaluate params from last label
    evaluate_by_mode(current_mode, params, c_params, objects);
}

