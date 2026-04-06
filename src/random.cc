#include "util.h"

#include <cstdlib>

double random_double() {
    return std::rand() / (RAND_MAX + 1.0);
}


