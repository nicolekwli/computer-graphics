#include "Raytracer.h"

// ----- Functions -----
void getClosestIntersection(ModelTriangle t, Camera cam){

    vec3 e0 = t.vertices[1] - t.vertices[0];
    vec3 e1 = t.vertices[2] - t.vertices[0];
    // vec3 SPVector = cam.cameraPos - t.vertices[0];
    // mat3 DEMatrix(-rayDirection, e0, e1);
    // vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

}


void drawFilledTriangleRay(DrawingWindow window, vector<ModelTriangle> t, Camera cam){

    getClosestIntersection(t[0], cam);

    // -The basic principle is to loop through each pixel on the image plane (top-to-bottom, left-to-right)
    // -casting a ray from the camera, through the current pixel and into the scene.
    // Your task is to determine if this ray intersects with a triangle from the model.
    
}


