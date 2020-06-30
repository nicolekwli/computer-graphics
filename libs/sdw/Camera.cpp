#include "Camera.h"

// empty constructor to create class
// Camera::Camera(void){
//     cout<<"object created"<<endl;
// }

void Camera::camForward(){
}

void Camera::camBackward(){
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


