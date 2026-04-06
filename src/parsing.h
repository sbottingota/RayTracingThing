#ifndef PARSING_H
#define PARSING_H

#include <string>

#include "camera.h"
#include "screen_object.h"

void parse_file(std::string filename, camera_params& c_params, object_group& world);

#endif
