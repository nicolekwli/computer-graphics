#include "Raytracer.h"


RayTriangleIntersection getClosestIntersection(vector<ModelTriangle> triangles, Camera cam, vec3 rayDirection){
    RayTriangleIntersection closest;
    float prevDist = 0; //a high value?

    // loop through each triangle
    for(int i=0; i<(int)triangles.size(); i++){
        // calc. the ray
        vec3 e0 = triangles[i].vertices[1] - triangles[i].vertices[0];
        vec3 e1 = triangles[i].vertices[2] - triangles[i].vertices[0];
        vec3 SPVector = cam.cameraPos - triangles[i].vertices[0];
        mat3 DEMatrix(-rayDirection, e0, e1);
        vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

        bool validSol = checkConstraints(possibleSolution);

        // if it is a valid solution
        if (validSol == true){
            float curDist = possibleSolution[0];
            // if current ray to triangle is shorter than prev then it is the closest
            if (curDist < prevDist){
                vec3 point = cam.cameraPos + curDist*rayDirection;
                closest = RayTriangleIntersection(point, curDist, triangles[i]);
            }
        }
    }

    return closest;
}


void drawFilledTriangleRay(DrawingWindow window, vector<ModelTriangle> triangles, Camera cam){
    // -The basic principle is to loop through each pixel on the image plane (top-to-bottom, left-to-right)
    // -casting a ray from the camera, through the current pixel and into the scene.
    // Your task is to determine if this ray intersects with a triangle from the model.
    RayTriangleIntersection closest;

    for(int y=0; y<window.height; y++){
        for(int x=0; x<window.width; x++){
            glm::vec3 rayDirection = vec3(x-window.width/2, y-window.height/2, cam.focalLength);
            closest = getClosestIntersection(triangles, cam, rayDirection);

            //if there is intersection
            if (closest.distanceFromCamera != 0){
                uint32_t pixelColour = bitpackingColour(closest.intersectedTriangle.colour);
                window.setPixelColour(x, y, pixelColour);
            }
            //else black
            else {
                uint32_t pixelColour = bitpackingColour(Colour(0,0,0));
                window.setPixelColour(x, y, pixelColour);
            }
        }
    }
}


/*
    // 0.0 <= u <= 1.0
    // 0.0 <= v <= 1.0
    // u + v <= 1.0
*/
bool checkConstraints(vec3 sol){
    float u = sol[1];
    float v = sol[2];
    bool result = false;

    if (0.0 <= u and u <= 1.0) {
        result = true;
    }
    else if (0.0 <= v and v <= 1.0) {
        result = true;
    }
    else if ((u + v) <= 1.0) {
        result = true;
    }

    //or do 
    // if ( (0.0 <= u and u <= 1.0) and (0.0 <= v and v <= 1.0) and ((u + v) <= 1.0) )

    return result;
}
