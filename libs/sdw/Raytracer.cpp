#include "Raytracer.h"


bool getClosestIntersection(vector<ModelTriangle> triangles, Camera cam, vec3 rayDirection){
    RayTriangleIntersection closest;
    float prevDist = 0; //a high value?

    // loop through each triangle
    for(int i=0; i<(int)triangles.size(); i++){
        // calc. the ray
        vec3 e0 = triangles[i].vertices[1] - triangles[i].vertices[0];
        vec3 e1 = triangles[i].vertices[2] - triangles[i].vertices[0];
        vec3 SPVector = startPos - triangles[i].vertices[0];
        mat3 DEMatrix(-rayDirection, e0, e1);
        vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

        // bool validSol = checkConstraints(possibleSolution);

        // // if it is a valid solution
        // if (validSol == true){
        //     float curDist = possibleSolution[0];
        //     // if current ray to triangle is shorter than prev then it is the closest
        //     if (curDist < prevDist){
        //         vec3 point = startPos + curDist*rayDirection;
        //         closest = RayTriangleIntersection(point, curDist, triangles[i]);
        //     }
        // }
        float u = possibleSolution.y;
        float v = possibleSolution.z;

        if (u >= 0.0f && u <= 1.0f && v >= 0.0f && v <= 1.0f && u + v <= 1) {
            if (possibleSolution.x < prevDist && possibleSolution.x > 0.0f) {
                closest = RayTriangleIntersection(startPos + (possibleSolution.x * rayDirection) , possibleSolution.x, triangles[i]);
                found = true;
                prevDist = possibleSolution.x;
            }
        }
    }

    return found;
}


// this version doesnt crash and actually shows something
// moller - trombone algorithm
// pseudocode from https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
bool getClosestInt(vector<ModelTriangle> triangles, vec3 startPos, vec3 rayDirection, RayTriangleIntersection &closest ){
    float prevDist = 1000; //a high value?
    rayDirection = glm::normalize(rayDirection);

    // loop through each triangle
    for(int i=0; i<(int)triangles.size(); i++){
        vec3 e01 = triangles[i].vertices[1] - triangles[i].vertices[0];
        vec3 e02 = triangles[i].vertices[2] - triangles[i].vertices[0];
        vec3 pvec = glm::cross(rayDirection, e02); 
        float determinant = glm::dot(e01, pvec);
        if (determinant < 0.001){
            continue; //return false?
        }
        
        float inverse = 1/determinant;

        vec3 tvec = startPos - triangles[i].vertices[0];
        float u = glm::dot(tvec, pvec) * inverse;
        vec3 qvec = glm::cross(tvec, e01);
        float v = glm::dot(rayDirection, qvec) * inverse;

        // checking condi,tions
        if (u < 0.0 || u > 1.0) continue;
        // if (v < 0.0 || v > 1.0) continue;
        if (v < 0.0 || u + v > 1) continue;
        // if ( (0.0 <= u && u <= 1.0) && (0.0 <= v && v <= 1.0) && ((u + v) <= 1.0)) continue;
    
        float t = glm::dot(e02, qvec) * inverse; 

        if (t < prevDist && t > 0 ){ //&& triangles[i] != self ????
            prevDist = t;
            closest = RayTriangleIntersection(startPos + t * rayDirection, prevDist, triangles[i]);
            return true;
        }
    }

    return false;
}


// -The basic principle is to loop through each pixel on the image plane (top-to-bottom, left-to-right)
// -casting a ray from the camera, through the current pixel and into the scene.
// Your task is to determine if this ray intersects with a triangle from the model.
void drawFilledTriangleRay(DrawingWindow window, vector<ModelTriangle> triangles, Camera cam){
    RayTriangleIntersection closest;

    for(int y=0; y<window.height; y++){
        for(int x=0; x<window.width; x++){
            glm::vec3 rayDirection = vec3(x-window.width/2, window.height/2-y, cam.focalLength) * cam.cameraRot;

            bool res = getClosestInt(triangles, cam.cameraPos, rayDirection, closest);
            // bool res = getClosestIntersection(triangles, cam.cameraPos, rayDirection, closest);

            //if there is intersection
            if (res){
                uint32_t pixelColour = bitpackingColour(closest.intersectedTriangle.colour);
                window.setPixelColour(x, y, pixelColour);
            }
            else { //else black
                uint32_t pixelColour = bitpackingColour(Colour(0,0,0));
                window.setPixelColour(x, y, pixelColour);
            }
        }
    }
}


uint32_t convertColour(Colour c, float brightness){
  int r   = std::min((int)(c.red   * brightness), 255);
  int g = std::min((int)(c.green * brightness), 255);
  int b  = std::min((int)(c.blue  * brightness), 255);
  return (255<<24) + (r<<16) + (g<<8) + b;
}

float ambientLighting(float brightness){
// set a minimum threshold (floor) for the brightness multiplier
// An IF statement will do the job - when the brightness of a pixel falls below a certain value (e.g. 0.2) just reset it to 0.2 !
    float thresh = 0.2;
    if (brightness < thresh){
        return thresh;
    }

    return thresh;
}

// raytracing with lighting
// diffuse and ambient
void raytracingLighting(DrawingWindow window, vector<ModelTriangle> triangles, Camera cam){
    RayTriangleIntersection closest;

    for(int y=0; y<window.height; y++){
        for(int x=0; x<window.width; x++){
            glm::vec3 rayDirection = vec3(x-window.width/2, window.height/2-y, cam.focalLength) * cam.cameraRot;

            bool res = getClosestInt(triangles, cam.cameraPos, rayDirection, closest);

            //if there is intersection
            if (res){
                uint32_t pixelColour = bitpackingColour(closest.intersectedTriangle.colour);
                float diffuseB = diffuseLighting(closest);
                float ambientB = ambientLighting(diffuseB);
                float totalB = diffuseB + ambientB;
                uint32_t finalColour = convertColour(Colour(255,255,255), totalB);
                window.setPixelColour(x, y, finalColour);
            }
            else { //else black
                uint32_t pixelColour = bitpackingColour(Colour(0,0,0));
                window.setPixelColour(x, y, pixelColour);
            }
        }
    }
}


// the closer a surface is to the light, the brighter a pixel will be drawn on the image plane
float diffuseLighting(RayTriangleIntersection intersection){
    // vec3 lightPos = glm::vec3( 0, -0.5, -0.7 );
    vec3 lightPos = vec3(0, 3, -FOCAL);
    vec3 lightColor = 50.f * glm::vec3( 1, 1, 1 ); //this is the power ??
    
    vec3 dirLight = lightPos - intersection.intersectionPoint;
    vec3 e01 = intersection.intersectedTriangle.vertices[1] - intersection.intersectedTriangle.vertices[0];
    vec3 e02 = intersection.intersectedTriangle.vertices[2] - intersection.intersectedTriangle.vertices[0];
    vec3 surfaceNormal = glm::cross(e01, e02);
    surfaceNormal = glm::normalize(surfaceNormal);
    float check = glm::dot(surfaceNormal, dirLight);
    float D = (lightColor.y * std::max(check, 0.f)) / (4 * pi * glm::dot(dirLight, dirLight));

    return D;

    //return colour * power / (4.0f * (static_cast<float>(M_PI)) * ray.getLength() * ray.getLength());
}


/* constraints are:
    // 0.0 <= u <= 1.0
    // 0.0 <= v <= 1.0
    // u + v <= 1.0
*/
bool checkConstraints(vec3 sol){
    float u = sol.y;
    float v = sol.z;
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
