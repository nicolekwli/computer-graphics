#include "Render.h"

#define WIDTH 640
#define HEIGHT 480

// setting up variables
DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
Camera mycam = Camera(HEIGHT, WIDTH);
PPM ppm;
int frame_count = 000;
int render_type = 1; //1 - wireframe, 2 - rasterizer, 3 - raytracer

// normal reading files
vector<Colour> cornell_mtl = readMTL("assets/cornell-box/cornell-box.mtl");
vector<ModelTriangle> cornellbox = readOBJ("assets/cornell-box/cornell-box.obj", cornell_mtl, ppm, 1);
vector<Colour> logo_mtl = readMTL("assets/hackspaceLogo/materials.mtl");
vector<ModelTriangle> logo = readOBJ("assets/hackspaceLogo/logo.obj", logo_mtl, ppm, 0.005);

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

        // display things to screen
        draw();

        // saving each frame as PMM ((might want to save in a folder?))
        // savePPM(window, "wireframe-"+to_string(frame_count)+".ppm");

        window.renderFrame();
        frame_count++;
    }
}

// Function for performing animation (shifting artifacts or moving the camera)
void update(){
    //could do the thing r did?? eeee
}

//currently just does stuff for wireframe
void draw(){
    ppm = readPPM(window, "assets/texture.ppm");
    cornellboxAnimate();
    // clearScreen();
    // logoFlyThrough();
}

// zooms into cornell box and cornell box flies out of screen
void cornellboxAnimate(){
    createWireframe(window, cornellbox, mycam);
    for (int i=0; i<=3; i++) mycam.camForward();
    for (int i=0; i<=2; i++) mycam.camBackward();
    // for (int i=0; i<=5; i++){
    //     // mycam.camRight();
    //     mycam.camOrientation(vec3(0, 0.01, 0));
    // }
    // for (int i=0; i<=5; i++){
    //     // mycam.camLeft();
    //     mycam.camOrientation(vec3(0, -0.01, 0));
    // }
}

void logoFlyThrough(){
    // if (mycam.cameraPos.z < -9.3){
    //     mycam.camForward();
    // }
    createWireframe(window, cornellbox, mycam);
    for (int i=0; i<=6; i++) mycam.camRight();
    for (int i=0; i<=5; i++) mycam.camForward();
}

void logoOrbit(){
}

// 'clears' screen by filling in pixels with black
void clearScreen(){
    for(int y=0; y<window.height; y++){
        for(int x=0; x<window.width; x++){
            window.setPixelColour(x, y, bitpackingColour(Colour(0,0,0)));
        }
    }
}

// use this later on when need to switch between the types
// in draw function?
void renderType(int type){
    switch(type){
        case 1: // wireframe
            break;
        case 2: // rasterizer
            break;
        case 3: // raytracer
            break;
        default: //wireframe is default
            break;
    }
}


//  triggered by code rather than keyboard buttons
// call like -> for (...) { handleEvent("cam_left"); } ???
void handleEvent(string event){
    if(event == "cam_left"){
        mycam.camLeft();
    }
    else if(event == "cam_right"){
        mycam.camRight();
    }
    else if(event == "cam_up"){
        mycam.camUp();
    } 
    else if(event == "cam_down"){
        mycam.camDown();
    }
    else if (event == "cam_forward"){
        mycam.camForward();
    } 
    else if (event == "cam_backward"){
        mycam.camBackward();
    } 
    // else if (event.key.keysym.sym == SDLK_w){
    //     mycam.camOrientation(vec3(0.01, 0, 0));
    // } 
    // else if (event.key.keysym.sym == SDLK_s){
    //     mycam.camOrientation(vec3(-0.01, 0, 0));
    // } 
    // else if (event.key.keysym.sym == SDLK_a){
    //     mycam.camOrientation(vec3(0, 0.01, 0));
    // } 
    // else if (event.key.keysym.sym == SDLK_d){
    //     mycam.camOrientation(vec3(0, -0.01, 0));
    // } 
    // else if (event.key.keysym.sym == SDLK_l){
    //     mycam.lookAt(vec3(0, 0, 0));
    // } 
}