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
    // Camera mycam;
    //cam.cameraRot[1][1] = 1.0f;

    vec3 point = cam.cameraRot * (vertex3D - cam.cameraPos) ;
    
    float x = point.x;
    float y = point.y;
    float z = point.z;

    float x2D = (cam.focalLength * x / (z)) + (window.width/2);
    float y2D = (window.height/2) - (cam.focalLength * y / (z));
    CanvasPoint vertex2D = CanvasPoint(x2D, y2D, 1/z);

    return vertex2D;
}

void modelToCanvasTri(DrawingWindow window, ModelTriangle mt, CanvasTriangle &ct, Camera cam){
    CanvasPoint v0 = vertex3Dto2D(window, mt.vertices[0], cam);
    CanvasPoint v1 = vertex3Dto2D(window, mt.vertices[1], cam);
    CanvasPoint v2 = vertex3Dto2D(window, mt.vertices[2], cam);
    ct = CanvasTriangle(v0, v1, v2, mt.colour);
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

void rasterise(DrawingWindow window, vector<ModelTriangle> t, Camera cam){
    for (std::vector<int>::size_type i = 0; i != t.size(); i++){
        CanvasTriangle ct; 
 
        modelToCanvasTri(window, t[i], ct, cam);
        drawFilledTriangle(window, ct.colour, ct);
    }
}