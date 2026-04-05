#include <iostream>

#define WIDTH 256
#define HEIGHT 256

int main() {
    std::cout << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

    for (int r = 0; r < HEIGHT; ++r) {
        for (int g = 0; g < WIDTH; g++) {
            std::cout << r << ' ' << g << ' ' << 0 << '\n';
        }
    }
}

