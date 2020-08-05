#pragma once

#include "CanvasPoint.h"
#include "CanvasTriangle.h"
#include "ModelTriangle.h"
#include "Colour.h"
#include "Helper.h"
#include "Utils.h"
#include <glm/glm.hpp>
#include <vector>

// 7.5f for cornell box
#define FOCAL 7.5f
//#define FOCAL_LENGTH HEIGHT/FOCAL
#define CAMERA_VELOCITY 0.4f

// Maybe this should be in main
using namespace std;
using namespace glm;

using std::vector;
using glm::vec3;


class Camera {
    public:
        // init values shoudl be in cpp when camera is created
        vec3 cameraPos;
        mat3 cameraRot;
        vec3 cameraFor;
        vec3 cameraRig;
        float focalLength;
        float yaw; //yaw angle controls cam rotation arounf y-axis
        float nearDist;
        float farDist;
        float fov;
        // frustum f;

        //member functions
        Camera(int height, int width);

        void camUp();
        void camDown();
        void camLeft();
        void camRight();
        void camForward();
        void camBackward();
        void camOrientation(vec3 rot);
        void lookAt(glm::vec3 from);
        // void setupFrus(int height, int width);
};