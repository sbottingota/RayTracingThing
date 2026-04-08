#include "parsing_util.h"

#include <regex>


void trim(std::string& s) {
    std::regex trim_regex("^\\s+|\\s+$");
    s = std::regex_replace(s, trim_regex, "");
}

void error_and_exit(std::string line, int line_no) {
    std::clog << "Line " << line_no << ": '" << line << "'\n";
    std::exit(1);
}


parameter::parameter(std::string line, int line_no) : line(line), line_no(line_no) {
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

void parameter::check_args(std::string expected_args) const {
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

double parameter::as_double() const {
    check_args("d");
    return std::get<double>(args[0]);
}

std::string parameter::as_string() const {
    check_args("s");
    return std::get<std::string>(args[0]);
}

vec3 parameter::as_vec3() const {
    vec3 vec;
    check_args("ddd");

    for (int i = 0; i < 3; ++i) {
        vec[i] = std::get<double>(args[i]);
    }

    return vec;
}

point3 parameter::as_point3() const {
    return as_vec3(); // note that vec3 and point3 are the same class under the hood
}

color parameter::as_color(int start) const {
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

color parameter::as_color() const {
    return as_color(0);
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

