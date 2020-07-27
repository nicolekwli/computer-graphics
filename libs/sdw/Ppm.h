#pragma once

#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;


class PPM {
    public:
        // init values shoudl be in cpp when camera is created
        int width;
        int height;
        vector<vector<uint32_t>> pixels;


        PPM(); //constructor
        PPM(int width, int height, vector<vector<uint32_t>> pixels);

};