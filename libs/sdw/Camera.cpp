#include "Camera.h"

// empty constructor to create class
// Camera::Camera(void){
//     cout<<"object created"<<endl;
// }
Camera::Camera(int height, int width){
    //cameraPos = vec3(0, 1, -6);
    cameraPos = vec3(0, 2, -FOCAL);
    //cameraRot = mat3(-1, 0, 0, 0, 1, 0, 0, 0, -1);
    //cameraRot = mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
    cameraRot = mat3(cos(0), 0, sin(0), 0, 1, 0, -sin(0), 0, cos(0));
    //cameraRot = mat3(1, 1, 1, 1, 1, 1, 1, 1, 1);
    // original is 480
    focalLength = 480/FOCAL;
    //focalLength = 480/24;
    yaw = 0;

    setupFrus(height, width);

    
}

void Camera::camUp(){
    cameraPos += (vec3(0,0.2,0) * cameraRot);
}

void Camera::camDown(){
    cameraPos -= (vec3(0,0.2,0) * cameraRot);
}

void Camera::camLeft(){
    cameraPos += (vec3(0.1,0,0) * cameraRot);
}

void Camera::camRight(){
    cameraPos -= (vec3(0.1,0,0) * cameraRot);
}


void Camera::camForward(){
    cameraPos += (vec3(0,0,0.1) * cameraRot);
}

void Camera::camBackward(){
    cameraPos -= (vec3(0,0,0.1) * cameraRot);
}

// this does big steps
void Camera::camOrientation(vec3 rot){
    mat3 rotateX = mat3(1, 0, 0, 0, cos(rot.x), sin(rot.x), 0, -sin(rot.x), cos(rot.x));
    mat3 rotateY = mat3(cos(rot.y), 0, -sin(rot.y), 0, 1, 0, sin(rot.y), 0, cos(rot.y));
    mat3 rotateZ = mat3(cos(rot.z), sin(rot.z), 0, -sin(rot.z), cos(rot.z), 0, 0, 0, 1);
    cameraRot = rotateX * rotateY * rotateZ * cameraRot;
}

void Camera::lookAt(glm::vec3 from){
    vec3 forward = glm::normalize(from - cameraPos);
    vec3 right = glm::normalize(glm::cross(vec3(0, 1, 0), forward));
    vec3 up = glm::normalize(glm::cross(forward, right));

    //cameraRot = vec3(right.x, right.y, right.z, up.x, up.y, up.z, forward.x, forward.y, forward.z);
    cameraRot[0][0] = right.x;
    cameraRot[1][0] = right.y;
    cameraRot[2][0] = right.z;
    cameraRot[0][1] = up.x;
    cameraRot[1][1] = up.y;
    cameraRot[2][1] = up.z;
    cameraRot[0][2] = forward.x;
    cameraRot[1][2] = forward.y;
    cameraRot[2][2] = forward.z;
}

void Camera::setupFrus(int height, int width){
    float angle_horizontal =  atan2(width/2,focalLength)-0.0001;
    float angle_vertical   =  atan2(height/2,focalLength)-0.0001;
    float sh               =  sin(angle_horizontal);
    float sv               =  sin(angle_vertical);
    float ch               =  cos(angle_horizontal);
    float cv               =  cos(angle_vertical);

    // left
    f.sides[0].normal.x=ch;
    f.sides[0].normal.y=0;
    f.sides[0].normal.z=sh;
    f.sides[0].distance = 0;
    // right
    f.sides[1].normal.x=-ch;
    f.sides[1].normal.y=0;
    f.sides[1].normal.z=sh;
    f.sides[1].distance = 0;
    // top
    f.sides[2].normal.x=0;
    f.sides[2].normal.y=cv;
    f.sides[2].normal.z=sv;
    f.sides[2].distance = 0;
    // bottom
    f.sides[3].normal.x=0;
    f.sides[3].normal.y=-cv;
    f.sides[3].normal.z=sv;
    f.sides[3].distance = 0;
    // z-near clipping plane
    f.znear.normal.x=0;
    f.znear.normal.y=0;
    f.znear.normal.z=1;
    f.znear.distance = -10;
    // far-plane
    f.zfar.normal.x = 0;
    f.zfar.normal.y = 0;
    f.zfar.normal.z = -1;
    f.zfar.distance = 1000;
}

void Camera::translate(float xpos, float ypos, float zpos){
    // the below cant work?
    // cameraPos = cameraRig - (cameraRig * 2.0f);

    // need to use transformation matrix or something to update the movement of camera
    cout<< xpos << " " << ypos << endl;
}

void TransformationMatrix(glm::mat4x4 M){
    // P is point in world space
    // C is where the camera is relative to the coordinate system
    // P-C is the point in the coordinate system centered at 0,0
    // R is rotation matrix
    // P' = R. (P-C)
}


//  homogenous coordinates!!! ???


