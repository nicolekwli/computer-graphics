#include "Raytracer.h"

// -------------------- HELPER FUNCS
/*  // 0.0 <= u <= 1.0
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


uint32_t convertColour(Colour c, float brightness){
  int r   = std::min((int)(c.red   * brightness), 255);
  int g = std::min((int)(c.green * brightness), 255);
  int b  = std::min((int)(c.blue  * brightness), 255);
  return (255<<24) + (r<<16) + (g<<8) + b;
}

// -------------------- OTHER FUNCIONS lol
// this doesnt work but could work (its too slow?)
bool getClosestIntersection(vector<ModelTriangle> triangles, vec3 startPos, vec3 rayDirection, RayTriangleIntersection &closest){
    float prevDist = 1000; //a high value? infinity?
    bool found = false;

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
        //     if (curDist < prevDist){
        //         vec3 point = startPos + curDist*rayDirection;
        //         closest = RayTriangleIntersection(point, curDist, triangles[i]);
        //     }
        // }
        float t = possibleSolution.x;
        float u = possibleSolution.y;
        float v = possibleSolution.z;

        // check constraints
        if (u >= 0.0f && u <= 1.0f && v >= 0.0f && v <= 1.0f && u + v <= 1) {
            // if dist of current ray to triangle is shorter than prev then it is the closest
            if (t < prevDist && t > 0.0f) { //&& triangles[i] != closest.intersectedTriangle
                closest = RayTriangleIntersection(startPos + (t * rayDirection), t, triangles[i]);
                found = true;
                prevDist = t;
            }
        }
    }
    return found;
}


// THIS WORKS!!!??? not exactly
// moller - trombone algorithm
// pseudocode from https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
bool closestIntersectionMT(vector<ModelTriangle> triangles, vec3 startPos, vec3 rayDirection, RayTriangleIntersection &closest ){
    float prevDist = 1000; //a high value?

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

        // checking conditions
        if (u < 0.0 || u > 1.0) continue;    
        if (v < 0.0 || u + v > 1) continue;
        // if ( (0.0 <= u && u <= 1.0) && (0.0 <= v && v <= 1.0) && ((u + v) <= 1.0)) continue;
    
        float t = glm::dot(e02, qvec) * inverse; 

        if (t < prevDist && t > 0 ){
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

    #pragma omp parallel for
    for(int y=0; y<window.height; y++){
        for(int x=0; x<window.width; x++){
            RayTriangleIntersection closest;

            glm::vec3 rayDirection = vec3(x-window.width/2, window.height/2-y, cam.focalLength) * cam.cameraRot;
            rayDirection = glm::normalize(rayDirection);

            // bool res = closestIntersectionMT(triangles, cam.cameraPos, rayDirection, closest);
            bool res = getClosestIntersection(triangles, cam.cameraPos, rayDirection, closest);

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

// set a minimum threshold (floor) for the brightness multiplier
// An IF statement will do the job - when the brightness of a pixel falls below a certain value (e.g. 0.2) just reset it to 0.2 !
float ambientLighting(float brightness){
   float thresh = 0.2;
    if (brightness < thresh){
        return thresh;
    }
    return thresh;
}

// the closer a surface is to the light, the brighter a pixel will be drawn on the image plane
float lighting(RayTriangleIntersection intersection, vec3 viewRay, vec3 &reflectedRay){
    float kd = 1; //material.diffuse.x;
    float ks = 0.55; //material.specular.x; //0.95;
    float ka = 1; //material.ambient.x;

    vec3 lightPos = glm::vec3( -0.25, 5, 3 ); // (0, -0.5, -0.7) light is in the light spot
    // vec3 lightPos = vec3(0, 1, -FOCAL); // light is where the camera is 
    vec3 lightColor = 50.f * glm::vec3( 1, 1, 1 ); //this is the power ??
    
    vec3 dirLight = lightPos - intersection.intersectionPoint; //intersection.intersectionPoint - lightPos;
    vec3 e01 = intersection.intersectedTriangle.vertices[1] - intersection.intersectedTriangle.vertices[0];
    vec3 e02 = intersection.intersectedTriangle.vertices[2] - intersection.intersectedTriangle.vertices[0];
    vec3 surfaceNormal = glm::cross(e01, e02);
    surfaceNormal = glm::normalize(surfaceNormal);
    float check = glm::dot(-surfaceNormal, dirLight); //not negative? yes negative
    float diffuse = (lightColor.z * std::max(check, 0.f)) / (4 * pi * glm::dot(dirLight, dirLight));
    // vec3 diff = (lightColor * std::max(check, 0.f)) / (4 * pi * glm::dot(dirLight, dirLight));

    // ambient lighting
    float ambient = ambientLighting(diffuse);

    // specular  lighting
    // math from wikipedia (https://en.wikipedia.org/wiki/Phong_reflection_model)
    float N = 100.f; // larger N -> smaller spot
    vec3 reflected = (2.0f * check * surfaceNormal) - dirLight;
    reflected = glm::normalize(reflected);
    reflectedRay = reflected; //passing reflected back
    float specular = std::max(glm::dot(reflected, viewRay), 0.f);

    // total light
    float totalLight = (ka * ambient) + (kd * diffuse) + (ks * powf(specular, N));

    return totalLight;
}


// for logo: no specular, no ambient, light pos: vec3 lightPos = vec3(0, 1, 3); ?
uint32_t findPixelColour(vector<ModelTriangle> &triangles, vec3 startpoint, vec3 rayDirection){
    RayTriangleIntersection closest;

    bool res = getClosestIntersection(triangles, startpoint, rayDirection, closest);

    if (res){
        float kd = 1; //material.diffuse.x;
        float ks = 1; //material.specular.x; //0.95;
        float ka = 0.3; //material.ambient.x;

        vec3 lightPos = glm::vec3( -0.25, 5, 3 ); // (0, -0.5, -0.7) light is in the light spot
        // vec3 lightPos = vec3(0, 1, -FOCAL); // light is where the camera is 
        vec3 lightColor = 50.f * glm::vec3( 1, 1, 1 ); //this is the power of light
        vec3 dirLight = lightPos - closest.intersectionPoint; //intersection.intersectionPoint - lightPos;
        dirLight = glm::normalize(dirLight);
        
        vec3 e01 = closest.intersectedTriangle.vertices[1] - closest.intersectedTriangle.vertices[0];
        vec3 e02 = closest.intersectedTriangle.vertices[2] - closest.intersectedTriangle.vertices[0];
        vec3 surfaceNormal = glm::cross(e01, e02);
        surfaceNormal = glm::normalize(surfaceNormal);

        vec3 reflected = rayDirection - (2.0f * glm::dot(rayDirection, surfaceNormal) * surfaceNormal); //its not dirLight here
        reflected = glm::normalize(reflected);
        ///........................................


        // illum 3 is mirror             
        if (closest.intersectedTriangle.objname == "floor"){
        // if (closest.intersectedTriangle.mat.illum == 3){
            // return findPixelColour(triangles, closest.intersectionPoint, reflected); //recursion!!!
            RayTriangleIntersection closestReflected;
            bool ares = getClosestIntersection(triangles, closest.intersectionPoint-(0.01f*surfaceNormal) , reflected, closestReflected); //- (0.01f * surfaceNormal) is for shadow acne

            if (ares){
                //the colour of the floor will be the colour of closestReflected
                return bitpackingColour(closestReflected.intersectedTriangle.colour);
            }
            else{
                return bitpackingColour(Colour(0,0,0)); //black
            }
        }
        
            Colour finalColour =  closest.intersectedTriangle.colour;

            // diffuse lighting
            float check = glm::dot(-surfaceNormal, dirLight);
            float diffuse = (lightColor.z * std::max(check, 0.f)) / (4 * pi * glm::dot(dirLight, dirLight));
            // ambient lighting
            float ambient = ambientLighting(diffuse);
            // specular  lighting
            float N = 100.f; // larger N -> smaller spot //maybe this should be in file?
            float specular = std::max(glm::dot(reflected, rayDirection), 0.f);
            // total light
            float totalLight = (kd * diffuse) + (ks * powf(specular, N));

            return convertColour(finalColour, totalLight);
        
    }
    else{
        return bitpackingColour(Colour(0,0,0)); //black
    }
}


// raytracing with diffuse and ambient and specular lighting
void raytracingLighting(DrawingWindow window, vector<ModelTriangle> triangles, Camera cam){

    #pragma omp parallel for
    for(int y=0; y<window.height; y++){
        for(int x=0; x<window.width; x++){
            glm::vec3 rayDirection = vec3(x-window.width/2, window.height/2-y, cam.focalLength) * cam.cameraRot;
            rayDirection = glm::normalize(rayDirection);

            uint32_t col = findPixelColour(triangles, cam.cameraPos, rayDirection);
            window.setPixelColour(x, y, col);
        }
    }
}
