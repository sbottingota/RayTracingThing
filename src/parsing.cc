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
};

void trim(std::string& s) {
    std::regex trim_regex("^\\s+|\\s+$");
    s = std::regex_replace(s, trim_regex, "");
}

void error_and_exit(std::string line, int line_no) {
    std::clog << "Line " << line_no << ": '" << line << "'\n";
    std::exit(1);
}

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

// TODO: implement
void evaluate_camera_params(std::vector<parameter>& params, camera& cam) {

}

void evaluate_sphere_params(std::vector<parameter>& params, object_group& objects) {
    point3 position;
    double size;

    // TODO: allow these to be configured
    color col(0.8, 0.8, 0.0);
    auto mat = std::make_shared<lambertian>(col);

    for (auto& param : params) {
        if (param.name == "position") {
            if (param.args.size() != 3) {
                std::clog << "'position' expected 3 arguments but got " << param.args.size() << '\n';
                error_and_exit(param.line, param.line_no);
            }

            try {
                for (int i = 0; i < 3; ++i) {
                    position[i] = std::get<double>(param.args[i]);
                }
            } catch (const std::bad_variant_access&) {
                std::clog << "'position' expected a double but got a string\n";
                error_and_exit(param.line, param.line_no);
            }

        } else if (param.name == "size") {
            if (param.args.size() != 1) {
                std::clog << "'size' expected 1 argument but got " << param.args.size() << '\n';
                error_and_exit(param.line, param.line_no);
            }

            try {
                size = std::get<double>(param.args[0]);
            } catch (const std::bad_variant_access&) {
                std::clog << "'size' expected a double but got a string\n";
                error_and_exit(param.line, param.line_no);
            }
        } else {
            std::clog << "Unknown parameter name '" << param.name << "'\n";
            error_and_exit(param.line, param.line_no);
        }
    }

    auto s = std::make_shared<sphere>(position, size, mat);
    objects.add(s);
}

void evaluate_by_mode(mode current_mode, std::vector<parameter>& params, camera& cam, object_group& objects) {
    switch (current_mode) {
        case mode::sphere:
            evaluate_sphere_params(params, objects);
            break;

        case mode::camera:
            evaluate_camera_params(params, cam);
            break;
    }
}

void parse_file(std::string filename, camera& cam, object_group& objects) {
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
            evaluate_by_mode(current_mode, params, cam, objects);
            params.clear();

            // evaluate the new label
            current_mode = parse_label(param);

        } else {
            params.push_back(param);
        }
    }

    // evaluate params from last label
    evaluate_by_mode(current_mode, params, cam, objects);
}

