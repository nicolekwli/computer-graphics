#include "Main.h"


#define WIDTH 640
#define HEIGHT 480

DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);

// creating a camera
Camera mycam = Camera();

int main(int argc, char* argv[]){
    SDL_Event event;
    while(true){
        if(window.pollForInputEvents(&event)) handleEvent(event);
        update();
        window.clearPixels();

        // vector<uint32_t> pixels = readPPM(window,"assets/texture.ppm");     
        //vector<float> res = interpolation( 2.2, 8.5, 7 );   
        // drawLine(window, CanvasPoint(100,20), CanvasPoint(20,200));

        //CanvasTriangle t = CanvasTriangle(CanvasPoint(160, 10), CanvasPoint(300, 230), CanvasPoint(10, 150));
        // t.vertices[0].texturePoint.x = 195;
        // t.vertices[0].texturePoint.y = 5;
        // t.vertices[1].texturePoint.x = 395;
        // t.vertices[1].texturePoint.y = 380;
        // t.vertices[2].texturePoint.x = 65;
        // t.vertices[2].texturePoint.y = 330;

        // vector<vector<uint32_t>>pixels = readPPM(window,"assets/texture.ppm");  
        // fillTextureTriangle(window, pixels, t);

        vector<Colour> c = readMTL("assets/cornell-box/cornell-box.mtl");
        vector<ModelTriangle> object = readOBJ("assets/cornell-box/cornell-box.obj", c);
        //createWireframe(window, object, mycam);
        rasterise(window, object, mycam);

        // savePPM(window, "hello.ppm");
        //vector<vector<uint32_t>>pixels = readPPM(window,"hello.ppm");  

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
    else if(event.key.keysym.sym == SDLK_u){
        // drawRandomTriangle();
        CanvasTriangle t = CanvasTriangle(CanvasPoint(rand()%300, rand()%150), CanvasPoint(rand()%300, rand()%150), CanvasPoint(rand()%300, rand()%150));
        drawStrokedTriangle(window, t), Colour(rand()%255, rand()%255, rand()%255);
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