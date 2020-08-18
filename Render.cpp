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
PPM ppm = readPPM(window, "assets/texture.ppm"); // texture: bricks texture1: tiger print
int frame_count = 120;
int render_type = 11; //1-3: wireframe, 4-8: rasterizer, 9-14: raytracer
string render = "rays";
int step = 0;

// cornell box
vector<Colour> cornell_mtl = readMTL("assets/cornell-box/cornell-box.mtl");
vector<ModelTriangle> cornellbox = readOBJ("assets/cornell-box/cornell-box.obj", cornell_mtl, ppm, 1);
//vector<Material> cornell_mtl_alt = readMTLAlt("assets/cornel-box/cornell-box.mtl"); //do we need

// --- one l is the edited mtl file, ll is the original
// cornell sphere original
vector<Material> cornell_mtl_alt = readMTLAlt("assets/cornel-box-extra/CornellBox-Sphere.mtl");
vector<ModelTriangle> cornellbox_alt = readOBJAlt("assets/cornel-box-extra/CornellBox-Sphere.obj", cornell_mtl_alt, ppm, 1.05); // this is black spheres

// cornell sphere edited mtl
vector<Material> cornell_mtl_alt_grey = readMTLAlt("assets/cornel-box-extra/CornellBox-Sphere-Grey.mtl");
vector<ModelTriangle> cornellbox_alt_grey = readOBJAlt("assets/cornel-box-extra/CornellBox-Sphere.obj", cornell_mtl_alt_grey, ppm, 1.05);

// material hackspacelogo (raster and wireframe)
vector<Colour> logo_mtl = readMTL("assets/hackspaceLogo/materials.mtl");
vector<ModelTriangle> logo = readOBJ("assets/hackspaceLogo/logo.obj", logo_mtl, ppm, 0.005);

// material hackspace logo (for rays, no texture) 
vector<Colour> logo_colour_mtl = readMTL("assets/hackspaceLogo/materials-flat-colour.mtl");
vector<ModelTriangle> logo_colour = readOBJ("assets/hackspaceLogo/logo.obj", logo_colour_mtl, ppm, 0.005);



// Compile to mp4 using: ffmpeg -f image2 -i %d.ppm -vcodec libx264 -pix_fmt yuv420p video.mp4
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
        

        window.renderFrame();
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
            else if (frame_count < 25){
                mycam.camRight();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }

            else if (frame_count < 40){
                if ((render == "rays") && (frame_count == 31)) render_type++; // to cornell box lighting
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 50){
                mycam.camRight();
                mycam.camBackward();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 58){
                mycam.camForward();
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 65){
                // if ((render == "rays") && (frame_count == 64)) render_type++; // dont need this?!
                mycam.camForward();
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 90){
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            
            else {
                render_type++;
                mycam.cameraPos = vec3(-1.5, 1, -2.5f);
                if (render == "rasterise") mycam.cameraPos = vec3(-1.5, 1, -2.5f);
                mycam.camOrientation(vec3(0.02, 0, 0));
                mycam.camLeft();
                mycam.camBackward();
                
                ::step++;
            }
            break;

        case 1: // sphere
            frame_count++;
            savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");

            if (frame_count < 108){
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 115){
                if ((render == "rasterise") && (frame_count == 109)) render_type++; // to gouraud
                if ((render == "rays") && (frame_count == 109)) render_type++; // to sphere w light
                mycam.camForward();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 120){
                mycam.camForward();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 130){ 
                if ((render == "rasterise") && (frame_count == 121)) render_type++; // to phong
                if ((render == "rays") && (frame_count == 121)) render_type++; //to sphere w mirror
                mycam.camBackward();
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 140){ 
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else {
                render_type++;
                mycam.cameraPos = vec3(0, 1, -4.5f);
                mycam.camRight();
                ::step++;
            }
            break;

        case 2: // switch to logo
            window.clearPixels();
            frame_count++;
            savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            mycam.camRight();
            ::step++;
            break;

        case 3: // logo animation
            if (frame_count < 152){
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 160){
                mycam.camOrientation(vec3(0, -0.01, 0));
                mycam.camForward();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 170){
                mycam.camOrientation(vec3(0, 0.01, 0));
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 185){
                mycam.camOrientation(vec3(0, -0.02, 0));
                mycam.camForward();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 193){
                mycam.camOrientation(vec3(0, -0.02, 0));
                mycam.camForward();
                mycam.camUp();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 200){
                mycam.camOrientation(vec3(0, 0.03, 0));
                mycam.camLeft();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 210){
                mycam.camDown();
                mycam.camOrientation(vec3(0, 0.04, 0));
                mycam.camBackward();
                mycam.camRight();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 225){
                mycam.camBackward();
                mycam.camRight();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 230){
                mycam.camOrientation(vec3(0, 0.04, 0));
                mycam.camBackward();
                mycam.camRight();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 245){
                mycam.camRight();
                frame_count++;
                savePPM(window, "renders/"+ render +"/"+to_string(frame_count)+".ppm");
            }
            else if (frame_count < 265){
                //mycam.camBackward();
                //mycam.camRight();
                mycam.lookAt(vec3(-4 + ( std::rand() % 3 ), -1 + ( std::rand() % 3 ), 0));
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



void draw(){
    

    switch(render_type){
        case 1: // wireframe -- box
            createWireframe(window, cornellbox, mycam);
            // cornellboxAnimate();
            // clearScreen();
            // logoFlyThrough();
            break;
        case 2: // wireframe -- sphere
            createWireframe(window, cornellbox_alt_grey, mycam);
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

        case 9:  // raytracer -- box fill
            drawFilledTriangleRay(window, cornellbox, mycam);
            break;
        case 10: // raytracer -- box w light
            raytracingCornell(window, cornellbox, mycam, false);
            break;
        case 11: // raytracer -- sphere fill
            drawFilledTriangleRay(window, cornellbox_alt_grey, mycam);            
            break;
        case 12: // raytracer -- sphere light
            raytracingCornell(window, cornellbox_alt_grey, mycam, false);
            break;
        case 13: // raytracer -- sphere mirror
            raytracingLighting(window, cornellbox_alt_grey, mycam);
            break;
        case 14: // raytracer -- logo colour and light
            raytracingCornell(window, logo_colour, mycam, true);
            break;
        default: //wireframe is default
            break;
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