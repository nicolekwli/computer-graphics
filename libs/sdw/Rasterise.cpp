#include "Rasterise.h"


// ALL NEED TO BE IN MAIN/ CAMERA
#define WIDTH 600
#define HEIGHT 600
#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512
#define FOCAL 7.5f
#define FOCAL_LENGTH SCREEN_HEIGHT/FOCAL
#define CAMERA_VELOCITY 0.4f

struct Camera { // struct to hold all camera variables
    vec3 cameraPos = vec3(0, 0, -FOCAL);
    mat3 cameraRot = mat3(-1, 0, 0, 0, 1, 0, 0, 0, -1);
    vec3 cameraFor;
    vec3 cameraRig;
};



CanvasPoint vertex3Dto2D(vec3 vertex3D) {
    Camera mycam;
    mycam.cameraRot[1][1] = 1.0f;

    vec3 point = (vertex3D - mycam.cameraPos) * mycam.cameraRot;
    
    float x = point.x;
    float y = point.y;
    float z = point.z;

    float x2D = (FOCAL_LENGTH * x / (z)) + (WIDTH/2);
    float y2D = (FOCAL_LENGTH * y / (z)) + (HEIGHT/2);
    CanvasPoint vertex2D = CanvasPoint(x2D, y2D, 1/z);

    return vertex2D;
}

void modelToCanvasTri(ModelTriangle mt, CanvasTriangle &ct){
    CanvasPoint v0 = vertex3Dto2D(mt.vertices[0]);
    CanvasPoint v1 = vertex3Dto2D(mt.vertices[1]);
    CanvasPoint v2 = vertex3Dto2D(mt.vertices[2]);
    ct = CanvasTriangle(v0, v1, v2, mt.colour);
}

void createWireframe(DrawingWindow window, vector<ModelTriangle> t){
    vector<CanvasTriangle> canvasTriangles; 
    // for each triangle we need to get the canvas triangle first
    // then draw each triangle
    for (std::vector<int>::size_type i = 0; i != t.size(); i++){
        CanvasTriangle ct; 
 
        modelToCanvasTri(t[i], ct);
        canvasTriangles.push_back(ct);

        // RASTERISING STARTS HERE
        // with each triangle done we loop through each pixel in the screen
        drawStrokedTriangle(window, canvasTriangles.back());


    }
}

void rasterise(DrawingWindow window, vector<ModelTriangle> t){
    for (std::vector<int>::size_type i = 0; i != t.size(); i++){
        CanvasTriangle ct; 
 
        modelToCanvasTri(t[i], ct);
        drawFilledTriangle(window, ct.colour, ct);
    }
}