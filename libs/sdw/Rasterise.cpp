#include "Rasterise.h"


// ALL NEED TO BE IN MAIN/ CAMERA
// #define WIDTH 600
// #define HEIGHT 600
// #define SCREEN_WIDTH 512
// #define SCREEN_HEIGHT 512
// #define FOCAL 7.5f
// #define FOCAL_LENGTH SCREEN_HEIGHT/FOCAL
// #define CAMERA_VELOCITY 0.4f


// depth buffer stuff? need to recalculate everytime with camera change
CanvasPoint vertex3Dto2D(DrawingWindow window, vec3 vertex3D, Camera cam) {

    vec3 point = (vertex3D - cam.cameraPos) * cam.cameraRot;
    //vec3 point = cam.cameraRot * (cam.cameraPos - vertex3D);
    
    float x = point.x;
    float y = point.y;
    float z = point.z;

    float x2D = (cam.focalLength * x / (z)) + (window.width/2);
    float y2D = (window.height/2) - (cam.focalLength * y / (z));

    CanvasPoint vertex2D = CanvasPoint(x2D, y2D, 1/z);

    // what happens if they are negative
    // do we need this

    // if (vertex2D.x < 0){
    //     vertex2D.x = 0;
    // } else if (vertex2D.x > window.width-1) {
    //     vertex2D.x = window.width-1;
    // }

    // if (vertex2D.y < 0){
    //     vertex2D.y = 0;
    // } else if (vertex2D.y > window.height-1){
    //     vertex2D.y = window.height-1;
    // }

    return vertex2D;
}

void modelToCanvasTri(DrawingWindow window, ModelTriangle mt, CanvasTriangle &ct, Camera cam){
    CanvasPoint v0 = vertex3Dto2D(window, mt.vertices[0], cam);
    CanvasPoint v1 = vertex3Dto2D(window, mt.vertices[1], cam);
    CanvasPoint v2 = vertex3Dto2D(window, mt.vertices[2], cam);
    ct = CanvasTriangle(v0, v1, v2, mt.colour);

    // each canvas point has a tp
    ct.vertices[0].texturePoint = mt.texturePoints[0];
    ct.vertices[1].texturePoint = mt.texturePoints[1];
    ct.vertices[2].texturePoint = mt.texturePoints[2];
}

void createWireframe(DrawingWindow window, vector<ModelTriangle> t, Camera cam){
    vector<CanvasTriangle> canvasTriangles; 
    // for each triangle we need to get the canvas triangle first
    // then draw each triangle
    for (std::vector<int>::size_type i = 0; i != t.size(); i++){
        CanvasTriangle ct; 
 
        modelToCanvasTri(window, t[i], ct, cam);
        canvasTriangles.push_back(ct);

        drawStrokedTriangle(window, canvasTriangles.back());
    }
}

// each triangle has a colour name ppm
void rasterise(DrawingWindow window, vector<ModelTriangle> t, Camera cam, vector<vector<uint32_t>> pixels){
    for (std::vector<int>::size_type i = 0; i != t.size(); i++){

        CanvasTriangle ct; 

        modelToCanvasTri(window, t[i], ct, cam);

        // if no texture
        if (ct.vertices[0].texturePoint.x == -1){
            drawFilledTriangle(window, ct.colour, ct);
        } else {
            fillTextureTriangle(window, pixels, ct);
        }   
    }
}