#include "CanvasPoint.h"
#include "CanvasTriangle.h"
#include "ModelTriangle.h"
#include "Colour.h"
#include "Helper.h"
#include "Utils.h"
#include <glm/glm.hpp>
#include <vector>

#define FOCAL 7.5f
#define FOCAL_LENGTH SCREEN_HEIGHT/FOCAL
#define CAMERA_VELOCITY 0.4f

// Maybe this should be in main
using namespace std;
using namespace glm;

using std::vector;
using glm::vec3;


class Camera {
    public:
        vec3 cameraPos = vec3(0, 0, -FOCAL);
        mat3 cameraRot = mat3(-1, 0, 0, 0, 1, 0, 0, 0, -1);
        vec3 cameraFor;
        vec3 cameraRig;
        float yaw = 0; //yaw angle controls cam rotation arounf y-axis

        //member functions
        // Camera(); //constructor
        void camForward();
        void camBackward();
        void translate(float xpos, float ypos, float zpos);
};