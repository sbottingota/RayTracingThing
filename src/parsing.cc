#include "parsing.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <cstdlib>

#include "parsing_util.h"
#include "material.h"


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
            c_params.max_depth = param.as_double();
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

