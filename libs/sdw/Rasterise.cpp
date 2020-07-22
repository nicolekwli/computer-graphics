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

vector<CanvasTriangle> clipping(DrawingWindow window, CanvasTriangle ct){
    vector<vec3> norm;
    norm.push_back(vec3(1,0,0)); 
    norm.push_back(vec3(-1,0,0)); 
    norm.push_back(vec3(0,1,0));
    norm.push_back(vec3(0,-1,0));
    norm.push_back(vec3(0,0,1));
    norm.push_back(vec3(0,0,-1));

    vector<CanvasTriangle> final;
    // for each normal
    for (int i=0; i<6; i++){
        float distance;
        vector<vec3> keep;
        vector<vec3> discard;

        // calc each point's distance to norm
        for (int j=0; j<3; j++){
            vec3 v = vec3(ct.vertices[j].x, ct.vertices[j].y, ct.vertices[j].depth);
            distance = dot(norm[i], v);

            if (distance >= 0){
                keep.push_back(v);
            } else discard.push_back(v);
        }

        if (keep.size() == 3) {
            final.push_back(ct);
            return final; // would returning ruin the whole thing
        }
        else if (discard.size() > 0){
            cout << 'here';
            return final; // would returning ruin the whole thing
        }
        // for some reason this should not have areturn till the end?
        // else if (keep.size() == 1){
        //     // get distance ratio to get 2 new points
        //     float ratio1 = dot(norm[i], keep[0])  / dot(norm[i], discard[0]);
        //     float ratio2 = dot(norm[i], keep[0])  / dot(norm[i], discard[1]);

        //     vec3 np1 = keep[0] + ratio1 * (discard[0] - keep[0]);
        //     vec3 np2 = keep[0] + ratio2 * (discard[1] - keep[1]);

        //     CanvasPoint newP1 = CanvasPoint(np1.x, np1.y, np1.z);
        //     CanvasPoint newP2 = CanvasPoint(np2.x, np2.y, np2.z);

        //     CanvasPoint p = CanvasPoint(keep[0].x, keep[0].y, keep[0].z);
        //     final.push_back(CanvasTriangle(p, newP1, newP2));
        //     //return final;
        // }
        // else if (keep.size() == 2){
        //     // get two triangles 
        //     float ratio1 = dot(norm[i], discard[0])  / dot(norm[i], keep[0]);
        //     float ratio2 = dot(norm[i], discard[0])  / dot(norm[i], keep[1]);

        //     vec3 np1 = keep[0] + ratio1 * (discard[0] - keep[0]);
        //     vec3 np2 = keep[0] + ratio2 * (discard[1] - keep[1]);

        //     CanvasPoint newP1 = CanvasPoint(np1.x, np1.y, np1.z);
        //     CanvasPoint newP2 = CanvasPoint(np2.x, np2.y, np2.z);

        //     CanvasPoint p1 = CanvasPoint(keep[0].x, keep[0].y, keep[0].z);
        //     CanvasPoint p2 = CanvasPoint(keep[1].x, keep[1].y, keep[1].z);
        //     final.push_back(CanvasTriangle(p1, p2, newP1));
        //     final.push_back(CanvasTriangle(p2, newP1, newP2));
        //     return final;
        // }
    }

    return final;
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
    vector<CanvasTriangle> canvasTriangles; 
    for (std::vector<int>::size_type i = 0; i != t.size(); i++){

        CanvasTriangle ct; 
        modelToCanvasTri(window, t[i], ct, cam);
        canvasTriangles.push_back(ct);

        vector<CanvasTriangle> cts = clipping(window, ct);

        for (int j = 0; j < cts.size(); j++){
            if (ct.vertices[0].texturePoint.x == -1){
                drawFilledTriangle(window, ct.colour, cts[j]);
            } else {
                fillTextureTriangle(window, pixels, cts[j]);
            }  
        }
    }

        
}