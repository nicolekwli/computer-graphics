#include "Camera.h"

Camera::Camera(int height, int width){
    //cameraPos = vec3(0, 1, -6);
    cameraPos = vec3(0, 2, -FOCAL); //(0, 0, -FOCAL)
    //cameraRot = mat3(-1, 0, 0, 0, 1, 0, 0, 0, -1);
    //cameraRot = mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
    cameraRot = mat3(cos(0), 0, sin(0), 0, 1, 0, -sin(0), 0, cos(0));
    //cameraRot = mat3(1, 1, 1, 1, 1, 1, 1, 1, 1);
    // original is 480
    focalLength = 480/FOCAL;
    //focalLength = 480/24;
    yaw = 0;

    nearDist = 1;
    farDist = 100;
    fov = 90.0f;

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

// referrenced from http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
void Camera::setupFrus(int height, int width){

    float ratio = width / height;

    // near plane height and width
    //float Hnear = 2 * tan(fov / 2) * nearDist;
    float Hnear = tan(fov / 2) * nearDist;
	float Wnear = Hnear * ratio;

    // far plane height and width
    float Hfar = 2 * tan(fov / 2) * farDist;
	float Wfar = Hfar * ratio;

    vec3 right = vec3(1,0,0) * cameraRot;
    vec3 up = vec3(0,1,0) * cameraRot;
    vec3 forward = vec3(0,0,1) * cameraRot;

    // vec3 fc = cameraPos + forward * farDist;
    // vec3 nc = cameraPos + forward * nearDist;

    // corners of each plane
	// vec3 ftl = fc + (up * Hfar/2) - (right * Wfar/2);
    // vec3 ftr = fc + (up * Hfar/2) + (right * Wfar/2);
	// vec3 fbl = fc - (up * Hfar/2) - (right * Wfar/2);
	// vec3 fbr = fc - (up * Hfar/2) + (right * Wfar/2);

    // vec3 ntl = nc + (up * Hnear/2) - (right * Wnear/2);
	// vec3 ntr = nc + (up * Hnear/2) + (right * Wnear/2);
	// vec3 nbl = nc - (up * Hnear/2) - (right * Wnear/2);
	// vec3 nbr = nc - (up * Hnear/2) + (right * Wnear/2);

    // Calculate normal and point of each plane

    // far / near center
    glm::vec3 fc = cameraPos + forward * farDist;
    f.sides[4].point = fc;
    f.sides[4].normal = -forward;
    // f.zfar.point = fc;
    // f.zfar.normal = -forward;

    vec3 nc = cameraPos + forward * nearDist;
    f.sides[5].point = nc;
    f.sides[5].normal = forward;
    // f.znear.point = nc;
    // f.znear.normal = forward;

    // top
    vec3 pos = (nc + up * (Hnear / 2)) - cameraPos;
    vec3 top = normalize(pos);
    top = cross(top, right);
    //vec3 pos = nc + (up * Hnear);
    //vec3 top = normalize(pos - cameraPos) * right;
    //vec3 top = normalize(cross((pos - cameraPos), right));
    f.sides[0].normal = top;
    f.sides[0].point = pos;

    // bottom
    pos = (nc - up * (Hnear / 2)) - cameraPos;
    vec3 bottom = -normalize(pos);
    bottom = cross(right, bottom);
    //pos = nc - (up * Hnear);
    //vec3 bottom = normalize(pos - cameraPos) * right;
    //vec3 bottom = -normalize(cross(right, (pos - cameraPos)));
    f.sides[1].normal = bottom;
    f.sides[1].point = pos;

    // left
    pos = (nc + right * (Wnear / 2)) - cameraPos;
    vec3 left = -normalize(pos);
    left = cross(left, up);
    //pos = nc + (right * Wnear);
    //vec3 left = normalize(pos - cameraPos) * up;
    //vec3 left = -normalize(cross((pos - cameraPos), up));
    f.sides[2].normal = left;
    f.sides[2].point = pos;

    // right
    pos = (nc - right * (Wnear / 2)) - cameraPos;
    left = normalize(pos);
    left = cross(up, left);
    //pos = nc - (right * Wnear);
    //left = normalize(pos - cameraPos) * up;
    //left = normalize(cross(up, (pos - cameraPos)));
    f.sides[3].normal = left;
    f.sides[3].point = pos;

    
}
