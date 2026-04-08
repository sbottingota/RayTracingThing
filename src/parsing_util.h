#ifndef PARSING_UTIL_H
#define PARSING_UTIL_H

#include <variant>
#include <string>
#include <vector>

#include "color.h"
#include "vec3.h"

void trim(std::string& s);

void error_and_exit(std::string line, int line_no);

class parameter {
    public:
    std::string name;
    std::vector<std::variant<double, std::string>> args;

    // for error messages
    std::string line;
    int line_no;


    parameter(std::string line, int line_no);

    // expected args should contain zero or more of 'd' or 's', representing double or string,
    // or + at the end to indicate that all following arguments should be ignored (e.g. in variadic parameters)
    void check_args(std::string expected_args) const;

    double as_double() const;
    std::string as_string() const;
    vec3 as_vec3() const;
    point3 as_point3() const;


    // note that these functions doesn't yet call check_args() (should probably change that in the future)
    color as_color(int start) const;
    color as_color() const;
};

enum class mode {
    sphere,
    camera
};

mode parse_label(parameter& param);

#endif
