#include "Render.h"
#include <chrono>
#include <thread>

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono;

#define WIDTH 640
#define HEIGHT 480

// setting up variables
DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
Camera mycam = Camera(HEIGHT, WIDTH);
PPM ppm = readPPM(window, "assets/texture.ppm");
int frame_count = 0;
int render_type = 4; //1 - wireframe, 4-8 - rasterizer, 3 - raytracer
string render = "rasterise";
int step = 0;

// normal reading files
vector<Colour> cornell_mtl = readMTL("assets/cornell-box/cornell-box.mtl"); //can prob get rid  of this?
//vector<Material> cornell_mtl_alt = readMTLAlt("assets/cornel-box/cornell-box.mtl"); // one l is the edited mtl file, ll is the original
vector<Material> cornell_mtl_alt = readMTLAlt("assets/cornel-box-extra/CornellBox-Sphere.mtl");
vector<Material> cornell_mtl_alt_grey = readMTLAlt("assets/cornel-box-extra/CornellBox-Sphere-Grey.mtl");

vector<ModelTriangle> cornellbox = readOBJ("assets/cornell-box/cornell-box.obj", cornell_mtl, ppm, 1);
vector<ModelTriangle> cornellbox_alt = readOBJAlt("assets/cornell-box-extra/CornellBox-Sphere.obj", cornell_mtl_alt, ppm, 1.05); // this is black spheres
vector<ModelTriangle> cornellbox_alt_grey = readOBJAlt("assets/cornell-box-extra/CornellBox-Sphere.obj", cornell_mtl_alt_grey, ppm, 1.05);


vector<Colour> logo_mtl = readMTL("assets/hackspaceLogo/materials.mtl");
vector<ModelTriangle> logo = readOBJ("assets/hackspaceLogo/logo.obj", logo_mtl, ppm, 0.005);

// material reading files
// vector<Material> m = readMTLAlt("assets/cornel-box-extra/CornellBox-Sphere.mtl");
// vector<ModelTriangle> object = readOBJAlt("assets/cornell-box-extra/CornellBox-Sphere.obj", m, ppm, 1);

// Compile to mp4 using ffmpeg -f image2 -i %d.ppm video.mp4
int main(int argc, char* argv[]){
    SDL_Event event;

    while(true){
        if(window.pollForInputEvents(&event)) handleEvent(event);

        // update the screen in steps
        update();

        window.clearPixels();
        window.clearDepth();

        // display things to screen
        draw();

        // saving each frame as PMM ((might want to save in a folder?))
        // savePPM(window, "wireframe-"+to_string(frame_count)+".ppm");

        window.renderFrame();
        //frame_count++;
        // if (frame_count > 20) break;
    }
}

// Function for performing animation (shifting artifacts or moving the camera)
void update(){
    switch(::step){
        case 0: // cornell box animation
            if (frame_count < 15){
                mycam.camForward();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 30){
                mycam.camRight();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }

            else if (frame_count < 50){
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 60){
                mycam.camRight();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 63){
                mycam.camDown();
                mycam.camForward();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 70){
                mycam.camForward();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 110){
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else {
                render_type++;
                mycam.cameraPos = vec3(-2, 1, -2.5f);
                if (render == "rasterise") mycam.cameraPos = vec3(-1.5, 1, -2.5f);
                mycam.camOrientation(vec3(0.02, 0, 0));
                mycam.camLeft();
                
                ::step++;
            }
            break;

        case 1: // sphere
            frame_count++;
            savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");

            if (frame_count < 150){
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 155){
                if ((render == "rasterise") && (frame_count == 151)) render_type++; // to gouraud
                mycam.camForward();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 160){
                mycam.camRight();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 175){ 
                if ((render == "rasterise") && (frame_count == 165)) render_type++; // to phong
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else {
                render_type++;
                mycam.cameraPos = vec3(0, 1, -4.5f);
                ::step++;
            }
            break;

        case 2: // switch to logo
            window.clearPixels();
            frame_count++;
            savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            ::step++;
            break;

        case 3: // logo animation
            // for (int i=0; i<=5; i++) {
            if (frame_count < 185){
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 205){
                mycam.camOrientation(vec3(0, -0.01, 0));
                mycam.camForward();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 215){
                mycam.camOrientation(vec3(0, 0.01, 0));
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 220){
                mycam.camOrientation(vec3(0, -0.04, 0));
                mycam.camForward();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 225){
                mycam.camOrientation(vec3(0, -0.02, 0));
                mycam.camForward();
                mycam.camUp();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 235){
                mycam.camOrientation(vec3(0, 0.03, 0));
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 245){
                mycam.camOrientation(vec3(0, 0.04, 0));
                mycam.camBackward();
                mycam.camRight();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 260){
                mycam.camBackward();
                mycam.camRight();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 265){
                mycam.camOrientation(vec3(0, 0.04, 0));
                mycam.camBackward();
                mycam.camRight();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 280){
                mycam.camRight();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 290){
                mycam.camBackward();
                mycam.camRight();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            
            else ::step++;
            break;

        case 4:
            exit(0); //stop updating, stop the code, stop everything
            break;
    }
}

//currently just does stuff for wireframe
// need to change camera variables for each tho
void draw(){
    

    switch(render_type){
        case 1: // wireframe -- box
            createWireframe(window, cornellbox, mycam);
            // cornellboxAnimate();
            // clearScreen();
            // logoFlyThrough();
            break;
        case 2: // wireframe -- sphere
            createWireframe(window, cornellbox_alt, mycam);
            break;
        case 3: // wireframe -- logo
            createWireframe(window, logo, mycam);
            break;
        case 4: // rasterizer -- box fill
            rasterise(window, cornellbox, mycam, ppm.pixels, cornell_mtl_alt, 1);
            break;
        case 5: // rasterizer -- sphere fill
            rasterise(window, cornellbox_alt_grey, mycam, ppm.pixels, cornell_mtl_alt, 1);
            break;
        case 6: // rasterizer -- sphere gouraud
            rasterise(window, cornellbox_alt_grey, mycam, ppm.pixels, cornell_mtl_alt, 3);
            break;
        case 7: // rasterizer -- sphere phong
            rasterise(window, cornellbox_alt, mycam, ppm.pixels, cornell_mtl_alt, 4);
            break;
        case 8: // rasterizer -- logo texture
            rasterise(window, logo, mycam, ppm.pixels, cornell_mtl_alt, 2);
            break;
        case 9: // raytracer
            drawFilledTriangleRay(window, cornellbox, mycam);
            break;
        case 10: // raytracer
            raytracingLighting(window, cornellbox, mycam);
            break;
        default: //wireframe is default
            break;
    }
}

// zooms into cornell box and cornell box flies out of screen
void cornellboxAnimate(){
    createWireframe(window, cornellbox, mycam);
    for (int i=0; i<=20; i++) {
        mycam.camForward();
        frame_count++;
        savePPM(window, "renders/wireframe/wireframe-"+to_string(frame_count)+".ppm");
    }
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


void handleEvent(SDL_Event event)
{
  if(event.type == SDL_KEYDOWN) {
    if(event.key.keysym.sym == SDLK_LEFT){
        cout << "LEFT" << endl;
        mycam.camLeft();
        frame_count++;
        savePPM(window, "renders/wireframe/"+to_string(frame_count)+".ppm");
    }
    else if(event.key.keysym.sym == SDLK_RIGHT){
        cout << "RIGHT" << endl;
        mycam.camRight();
        frame_count++;
        savePPM(window, "renders/wireframe/"+to_string(frame_count)+".ppm");
    }
    else if(event.key.keysym.sym == SDLK_UP){
        cout << "UP" << endl;
        mycam.camUp();
        frame_count++;
        savePPM(window, "renders/wireframe/"+to_string(frame_count)+".ppm");
    } 
    else if(event.key.keysym.sym == SDLK_DOWN){
        cout << "DOWN" << endl;
        mycam.camDown();
        frame_count++;
        savePPM(window, "renders/wireframe/"+to_string(frame_count)+".ppm");
    }
    else if (event.key.keysym.sym == SDLK_e){
        cout << "FORWARD" << endl;
        mycam.camForward();
        frame_count++;
        savePPM(window, "renders/wireframe/"+to_string(frame_count)+".ppm");
        // some sampel code for fly through
        // if (mycam.cameraPos.z < -9.3){
        //   mycam.camForward();
        // }
    } 
    else if (event.key.keysym.sym == SDLK_q){
        cout << "BACKWARD" << endl;
        mycam.camBackward();
    } 
    else if (event.key.keysym.sym == SDLK_w){
        cout << "ROTATE UP" << endl;
        mycam.camOrientation(vec3(0.01, 0, 0));
    } 
    else if (event.key.keysym.sym == SDLK_s){
        cout << "ROTATE down" << endl;
        mycam.camOrientation(vec3(-0.01, 0, 0));
    } 
    else if (event.key.keysym.sym == SDLK_a){
        cout << "ROTATE LEFT" << endl;
        mycam.camOrientation(vec3(0, 0.01, 0));
    } 
    else if (event.key.keysym.sym == SDLK_d){
        cout << "ROTATE RIGHT" << endl;
        mycam.camOrientation(vec3(0, -0.01, 0));
    } 
    else if (event.key.keysym.sym == SDLK_l){
        cout << "LOOK AT" << endl;
        mycam.lookAt(vec3(0, 0, 0));
    } 
    else if(event.key.keysym.sym == SDLK_u){
        // drawRandomTriangle();
        CanvasTriangle t = CanvasTriangle(CanvasPoint(rand()%300, rand()%150), CanvasPoint(rand()%300, rand()%150), CanvasPoint(rand()%300, rand()%150));
        drawStrokedTriangle(window, t);
        cout << "U" << endl;
    }
    else if (event.key.keysym.sym == SDLK_f){
        //drawFilledTriangle((window), Colour(rand()%255, rand()%255, rand()%255));
    }    
    else if(event.key.keysym.sym == SDLK_f){
     // drawRandomFilledTriangle();
    } 
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN) cout << "MOUSE CLICKED" << endl;
}