#include "Render.h"

#define WIDTH 640
#define HEIGHT 480

// setting up variables
DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
Camera mycam = Camera(HEIGHT, WIDTH);
PPM ppm;

// normal reading files
vector<Colour> c = readMTL("assets/cornell-box/cornell-box.mtl");
vector<ModelTriangle> object = readOBJ("assets/cornell-box/cornell-box.obj", c, ppm, 1);

// material reading files
// vector<Material> m = readMTLAlt("assets/cornel-box-extra/CornellBox-Sphere.mtl");
// vector<ModelTriangle> object = readOBJAlt("assets/cornell-box-extra/CornellBox-Sphere.obj", m, ppm, 1);


int main(int argc, char* argv[]){
    SDL_Event event;

    while(true){
        // if(window.pollForInputEvents(&event)) handleEvent(event);
        // update();
        window.clearPixels();
        window.clearDepth();

        ppm = readPPM(window,"assets/texture.ppm"); 
        createWireframe(window, object, mycam);
        
        // saving each frame as PMM
        // savePPM(window, "file1");

        window.renderFrame();        
    }
}


void renderType(int key){
    // switch(key):
    //     case 0: // wireframe
    //         createWireframe(window, object, mycam);

    //     case 1: // rasterizer

    //     case 2: // raytracer

    //     default: 
}
