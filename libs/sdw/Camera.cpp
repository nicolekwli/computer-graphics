#include "Camera.h"

// empty constructor to create class
// Camera::Camera(void){
//     cout<<"object created"<<endl;
// }
Camera::Camera(){
    cameraPos = vec3(0, 0, -FOCAL);
    //cameraRot = mat3(-1, 0, 0, 0, 1, 0, 0, 0, -1);
    cameraRot = mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
    focalLength = 480/FOCAL;
    yaw = 0;
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

void lookAt(){
    vec3 forward = glm::normalize(from - to);
    vec3 right = glm::normalize(glm::cross(vec3(0, 1, 0), forward));
    vec3 up = glm::normalize(glm::cross(forward, right));
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


