#include "Main.h"
#include "Helper.h"


#define WIDTH 640
#define HEIGHT 480

DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);

int main(int argc, char* argv[]){
    SDL_Event event;
    while(true){
        if(window.pollForInputEvents(&event)) handleEvent(event);
        update();
        // test read ppm
        //readPPM("assets/texture.ppm");     
        vector<float> res = interpolation( 2.2, 8.5, 7 );   

        window.renderFrame();
    }
}

// Function for performing animation (shifting artifacts or moving the camera)
void update() {
}

void handleEvent(SDL_Event event)
{
  if(event.type == SDL_KEYDOWN) {
    if(event.key.keysym.sym == SDLK_LEFT) cout << "LEFT" << endl;
    else if(event.key.keysym.sym == SDLK_RIGHT) cout << "RIGHT" << endl;
    else if(event.key.keysym.sym == SDLK_UP) cout << "UP" << endl;
    else if(event.key.keysym.sym == SDLK_DOWN) cout << "DOWN" << endl;
    else if(event.key.keysym.sym == SDLK_u){
        // drawRandomTriangle();
        cout << "U" << endl;
    }
    else if (event.key.keysym.sym == SDLK_f){
        // drawFilledTriangle();
    }    
    else if(event.key.keysym.sym == SDLK_f){
     // drawRandomFilledTriangle();
    } 
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN) cout << "MOUSE CLICKED" << endl;
}