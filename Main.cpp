#include "Main.h"


#define WIDTH 640
#define HEIGHT 480

// #define WIDTH 840
// #define HEIGHT 680

DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);

// creating a camera
Camera mycam = Camera(HEIGHT, WIDTH);
//vector<vector<uint32_t>> pixels;
PPM ppm;

// normal cornell box
vector<Colour> c = readMTL("assets/cornell-box/cornell-box.mtl");
vector<ModelTriangle> object = readOBJ("assets/cornell-box/cornell-box.obj", c, ppm, 1);

// material cornell box (for raytracing)
vector<Material> c_alt_ray = readMTLAlt("assets/cornell-box/cornell-box-ray.mtl");
vector<ModelTriangle> object_alt = readOBJAlt("assets/cornell-box/cornell-box-ray.obj", c_alt_ray, ppm, 1);

// material cornell sphere 
vector<Material> m = readMTLAlt("assets/cornel-box-extra/CornellBox-Sphere.mtl");
vector<ModelTriangle> object1 = readOBJAlt("assets/cornel-box-extra/CornellBox-Sphere.obj", m, ppm, 1);

// material hackspace logo (for rays, no texture) 
vector<Colour> logo_mtl = readMTL("assets/hackspaceLogo/materials-flat-colour.mtl");
vector<ModelTriangle> logo_colour = readOBJ("assets/hackspaceLogo/logo.obj", logo_mtl, ppm, 0.005);


int main(int argc, char* argv[]){
    SDL_Event event;
    while(true){
        if(window.pollForInputEvents(&event)) handleEvent(event);
        update();
        window.clearPixels();
        window.clearDepth();

    
        //vector<float> res = interpolation( 2.2, 8.5, 7 );   
        //drawLine(window, CanvasPoint(100,20), CanvasPoint(20,200));
        // drawLineB(window, CanvasPoint(10,200), CanvasPoint(200,10), Colour(12,120,120));
        // drawLineB(window, CanvasPoint(10,200), CanvasPoint(200,150), Colour(12,120,120));

        //CanvasTriangle t = CanvasTriangle(CanvasPoint(160, 10), CanvasPoint(300, 230), CanvasPoint(10, 150));
        // t.vertices[0].texturePoint.x = 195;
        // t.vertices[0].texturePoint.y = 5;
        // t.vertices[1].texturePoint.x = 395;
        // t.vertices[1].texturePoint.y = 380;
        // t.vertices[2].texturePoint.x = 65;
        // t.vertices[2].texturePoint.y = 330;

        //drawStrokedTriangle(window, t);

        //pixels = readPPM(window,"assets/texture.ppm");  
        ppm = readPPM(window,"assets/texture.ppm"); 
        // hackspace logo
        vector<Colour> l = readMTL("assets/hackspaceLogo/materials.mtl");
        vector<ModelTriangle> logo = readOBJ("assets/hackspaceLogo/logo.obj", l, ppm, 0.005);
        //ppm = readPPM(window,"assets/texture1.ppm"); 
        //fillTextureTriangle(window, ppm.pixels, t);

        // rasterizer (old)
        createWireframe(window, logo, mycam);
        //rasterise(window, logo, mycam, ppm.pixels);

        // savePPM(window, "hello.ppm");

        //raytracer stuff
        // drawFilledTriangleRay(window, object1, mycam);
        raytracingLighting(window, object1, mycam);
        // raytracingCornell(window, object1, mycam);

        // //createWireframe(window, object, mycam);
        // rasterise(window, object, mycam, ppm.pixels);

        // vector<Colour> c = readMTL("assets/Crate/Crate1.mtl");
        // vector<ModelTriangle> object = readOBJ("assets/Crate/Crate1.obj", c, ppm, 1);
        // rasterise(window, object, mycam, ppm.pixels);

        // rasterizer (new)
        //createWireframe(window, object1, mycam);
        //rasterise(window, object, mycam, ppm.pixels, m, 1); // fill
        // rasterise(window, logo, mycam, ppm.pixels, m, 2);
        //rasterise(window, object1, mycam, ppm.pixels, m, 3);  // shade gouraud
        // rasterise(window, object1, mycam, ppm.pixels, m, 4);  // shade phong
        //SSAA(window);

        window.renderFrame();
    }
}

// Function for performing animation (shifting artifacts or moving the camera)
void update() {
}


void handleEvent(SDL_Event event)
{
  if(event.type == SDL_KEYDOWN) {
    if(event.key.keysym.sym == SDLK_LEFT){
        cout << "LEFT" << endl;
        mycam.camLeft();
    }
    else if(event.key.keysym.sym == SDLK_RIGHT){
        cout << "RIGHT" << endl;
        mycam.camRight();
    }
    else if(event.key.keysym.sym == SDLK_UP){
        cout << "UP" << endl;
        mycam.camUp();
    } 
    else if(event.key.keysym.sym == SDLK_DOWN){
        cout << "DOWN" << endl;
        mycam.camDown();
    }
    else if (event.key.keysym.sym == SDLK_e){
        cout << "FORWARD" << endl;
        mycam.camForward();
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