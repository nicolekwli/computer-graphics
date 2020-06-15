#include <ModelTriangle.h>
#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;
//using namespace glm;
using glm::mat3;
using glm::vec3;
using glm::vec4;
using glm::mat4x4;

#define WIDTH 600
#define HEIGHT 600
#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512
#define FOCAL 7.5f
#define FOCAL_LENGTH SCREEN_HEIGHT/FOCAL
#define CAMERA_VELOCITY 0.4f

// ----------- STRUCTS --------------------------------------
// vec3 cameraPos = vec3(WIDTH/2, HEIGHT/2, -FOCAL);
// vec3 cameraOrigin = vec3(256, 256, -3.001);

struct Camera { // struct to hold all camera variables
    // centre of camera coordinate system is 0,0,0
    // vec4 cameraOrigin = vec4(0, 0, -3.001, 1);
    // vec4 cameraPos = vec4(0, 0, -3.001, 1); // this variable changes as you move the camera
    vec3 cameraPos = vec3(0, 0, -FOCAL);
    //vec3 cameraRot = vec3(0, 0, 0);
    mat3 cameraRot = mat3(-1, 0, 0, 0, 1, 0, 0, 0, -1);

    vec3 cameraFor;
    vec3 cameraRig;
};

struct colourPalette{
    string objectName;
    string materialName;
};

// ----------- FUNCION HEADERS ------------------------------
void update();
void handleEvent(SDL_Event event, Camera cam);

void readAndParseOBJ(string filename);
void readAndParseMTL(string filename);

uint32_t bitpackingColour(Colour c);

void drawPoints(CanvasTriangle t);
void drawStrokedTriangle(CanvasTriangle t);
vector<CanvasPoint> interpolationCanvasPoints(CanvasPoint a, CanvasPoint b, float noOfVal);

void modelToCanvasTri(ModelTriangle mt, CanvasTriangle &ct);
CanvasPoint vertex3Dto2D(vec3 vertex3D);
void transformationMatrix(mat4x4 M);
void createWireframe();

void calcRotMatrix(vec3 &rot, mat3 &R);


// ----------- GLOBAL VARIABLES ------------------------------
DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);

vector<colourPalette> palette;
vector<ModelTriangle> triangles;
vector<Colour> colours;
vector<glm::vec3> ver;
mat3 R;
Camera mycam;



// ----------- MAIN ------------------------------------------
int main(int argc, char* argv[]){
    SDL_Event event;

    readAndParseMTL("cornell-box/cornell-box.mtl");
    readAndParseOBJ("cornell-box/cornell-box.obj");

    //calcRotMatrix(mycam.cameraRot, R);
    //mycam.cameraFor = glm::transpose(R) * vec3(0, 0, 1);
    //mycam.cameraRig = glm::transpose(R) * vec3(1, 0, 0);
    mycam.cameraRot[1][1] = 1.0f;

    createWireframe();

    while(true) {
        if(window.pollForInputEvents(&event)) handleEvent(event, mycam);
        update();
        window.renderFrame();
    }
}

/*void calcRotMatrix(vec3 &rot, mat3 &R) {
  float sin_x = sinf(rot.x);
  float cos_x = cosf(rot.x);
  float sin_y = sinf(rot.y);
  float cos_y = cosf(rot.y);
  mat3 Rx = mat3(1, 0, 0, 0, cos_x, -sin_x, 0, sin_x, cos_x);
  mat3 Ry = mat3(cos_y, 0, sin_y, 0, 1, 0, -sin_y, 0, cos_y);
  R = Rx * Ry;
}*/

void readAndParseOBJ(string filename){
    // std::ifstream ifs ("cornell-box/cornell-box.obj", std::ifstream::in);
    ifstream file;
    file.open(filename);

    string line;
    string *tokens;
    Colour col;

    // get first line matllib
    getline(file, line);
    
    while (!file.eof()){
        getline(file, line);
        while (!line.empty()){

            tokens = split(line, ' ');
            colourPalette p = colourPalette();

            if (tokens[0] == "o"){
                p.objectName = tokens[1];
                getline(file, line);
            }
            else if (tokens[0] == "usemtl") {
                // p.materialName = tokens[1];
                // Colour
                for (std::vector<int>::size_type i = 0; i != colours.size(); i++){
                    if(colours[i].name == tokens[1]){
                        col = colours[i];
                    }
                }
           
                getline(file, line);
            } 
            else if (tokens[0] == "v"){
                glm::vec3 vec = vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
                ver.push_back(vec);
                getline(file, line);
            }
            else if (tokens[0] == "f"){
                int a = stoi(tokens[1].substr(0, tokens[1].size()-1)) -1;
                int b = stoi(tokens[2].substr(0, tokens[2].size()-1)) -1;
                int c = stoi(tokens[3].substr(0, tokens[3].size()-1)) -1;

                triangles.push_back(ModelTriangle(ver[a], ver[b], ver[c], col));

                getline(file, line);                
            } else {
                //cout << "wtf is this line" << endl;
            }
            // else if its the end of an object?        
        }
    }
    file.close();
}

void readAndParseMTL(string filename){
    ifstream file;
    file.open(filename);

    string line;
    string *tokens;
    string name;

    while (!file.eof()){
        getline(file, line);
        tokens = split(line, ' ');

        if (tokens[0] == "newmtl"){
            name = tokens[1];
        } 
        else if (tokens[0] == "Kd"){
            colours.push_back(Colour(name, int(255 * stof(tokens[1])), int(255 * stof(tokens[2])), int(255 * stof(tokens[3]))));
        } else {
            // else?
        }
    }
    file.close();
}

uint32_t bitpackingColour(Colour c){
  uint32_t converted;
  converted = (255<<24) + (c.red<<16) + (c.green<<8) + c.blue;

  return converted;
}

vector<CanvasPoint> interpolationCanvasPoints(CanvasPoint a, CanvasPoint b, float noOfVal){
    vector<CanvasPoint> vect;
    float intervalsX;
    float intervalsY;
    CanvasPoint p;
    bool swap = false;
    if (a.y > b.y){
        if (a.x < b.x){
            intervalsX = abs(a.x - b.x) / (noOfVal-1);
            intervalsY = abs(a.y - b.y) / (noOfVal-1);
        } else {
            swap = true; 
            intervalsX = (a.x - b.x) / (noOfVal-1);
            intervalsY = (a.y - b.y) / (noOfVal-1);
        }
    } else {
        if (a.x > b.x){
            swap = true;
            intervalsY = (a.y - b.y) / (noOfVal-1);
            intervalsX = abs(a.x - b.x) / (noOfVal - 1);
        } else {
            intervalsY = abs(a.y - b.y) /(noOfVal-1);
            intervalsX = abs(a.x - b.x) / (noOfVal-1);
        }
    }

    vect.push_back(a);
    for (int i=1; i<noOfVal; i++) {
      if(swap){
        p = CanvasPoint(b.x + intervalsX*i, b.y + intervalsY*i);
      } else{
        p = CanvasPoint(a.x + intervalsX*i, a.y + intervalsY*i);
      }
      vect.push_back(p);
    }
    return vect;
}

void drawPoints(CanvasTriangle t){
  //float steps = std::max(abs(p1.x - p2.x), abs(p1.y - p2.y));
  //vector<CanvasPoint> line = interpolationCanvasPoints(p1, p2, steps);
    
  /*for (int i=0; i<(int)steps; i++){
    uint32_t colour = 255;
    window.setPixelColour((int)line[i].x, (int)line[i].y, colour);
  }*/
  uint32_t colour = 4294967290;
  for (int i = 0; i < 3; i++){
      window.setPixelColour((int)t.vertices[i].x, (int)t.vertices[i].y, colour);
  }
  
}

void drawStrokedTriangle(CanvasTriangle t){
    uint32_t colour = (255<<24) + (t.colour.red << 16) + (t.colour.green << 8) + t.colour.blue;
    //uint32_t colour = bitpackingColour(t.colour);
    cout << t.colour << endl;
    // sort vertices
    for (int i = 0; i < 3; i++){
        if (t.vertices[2].y < t.vertices[0].y){
            swap(t.vertices[2], t.vertices[0]);
        }
        if (t.vertices[1].y < t.vertices[0].y){
            swap(t.vertices[0], t.vertices[1]);
        }
        if (t.vertices[2].y < t.vertices[1].y){
            swap(t.vertices[1], t.vertices[2]);
        }
    }

    // use interpolation to draw line
    float stepsAB = std::max(abs(t.vertices[0].x - t.vertices[1].x), abs(t.vertices[0].y - t.vertices[1].y));
    vector<CanvasPoint> lineAB = interpolationCanvasPoints(t.vertices[0], t.vertices[1], stepsAB);
   
    float stepsBC = std::max(abs(t.vertices[1].x - t.vertices[2].x), abs(t.vertices[1].y - t.vertices[2].y));
    vector<CanvasPoint> lineBC = interpolationCanvasPoints(t.vertices[1], t.vertices[2], stepsBC);
   
    float stepsAC = std::max(abs(t.vertices[0].x - t.vertices[2].x), abs(t.vertices[0].y - t.vertices[2].y));
    vector<CanvasPoint> lineAC = interpolationCanvasPoints(t.vertices[0], t.vertices[2], stepsAC);
    
    for (int i=0; i<(int)stepsAB; i++){
      window.setPixelColour((int)lineAB[i].x, (int)lineAB[i].y, colour);
    }
    for (int i=0.0; i<(int)stepsBC; i++){
      window.setPixelColour((int)lineBC[i].x, (int)lineBC[i].y, colour);
    }
    for (int i=0.0; i<(int)stepsAC; i++){
      window.setPixelColour((int)lineAC[i].x, (int)lineAC[i].y, colour);
    }
}

/*void fillTriangles(CanvasTriangle t){

}*/


// ORDER of transformations is RIGHT TO LEFT 
// implementing homogenous coordinates!!!
void TransformationMatrix(glm::mat4x4 M){
    // P is point in world space
    // C is where the camera is relative to the coordinate system
    // P-C is the point in the coordinate system centered at 0,0
    // R is rotation matrix
    // P' = R. (P-C)
}


// takes in a 3D vertex (vec3) and returns a 2D vertex (CanvasPoint)
CanvasPoint vertex3Dto2D(vec3 vertex3D) {
    //float yaw = (0.0f * 3.1415926 / 180);
    //mat3 R = mat3(cos(yaw), 0, sin(yaw), 0, 1, 0, -sin(yaw), 0, cos(yaw));
    //vec3 point = R * (vertex3D - mycam.cameraPos);

    vec3 point = (vertex3D - mycam.cameraPos) * mycam.cameraRot;
    
    float x = point.x;
    float y = point.y;
    float z = point.z;

    //adjust for translation of camera here
    // formula is from carls lab
    //float x2D = ((vertex3D.x)*FOCAL/(vertex3D.z)) + (WIDTH/2);
    //float y2D = ((vertex3D.y)*FOCAL/(vertex3D.z)) + (HEIGHT/2);

    float x2D = (FOCAL_LENGTH * x / (z)) + (WIDTH/2);
    float y2D = (FOCAL_LENGTH * y / (z)) + (HEIGHT/2);
    CanvasPoint vertex2D = CanvasPoint(x2D, y2D, 1/z);

    return vertex2D;
}


// each value is currenlt -1 ... 1
void modelToCanvasTri(ModelTriangle mt, CanvasTriangle &ct){
    // each vertex is a vec3 meaning x,y,z and z is the depth of each canvaspoint
    // colour should be the same
    // CanvasPoint v0 = CanvasPoint(mt.vertices[0].x, mt.vertices[0].y, mt.vertices[0].z);
    // CanvasPoint v1 = CanvasPoint(mt.vertices[1].x, mt.vertices[1].y, mt.vertices[1].z);
    // CanvasPoint v2 = CanvasPoint(mt.vertices[2].x, mt.vertices[2].y, mt.vertices[2].z);
    // ct = CanvasTriangle(v0, v1, v2, mt.colour);

    CanvasPoint v0 = vertex3Dto2D(mt.vertices[0]);
    CanvasPoint v1 = vertex3Dto2D(mt.vertices[1]);
    CanvasPoint v2 = vertex3Dto2D(mt.vertices[2]);
    ct = CanvasTriangle(v0, v1, v2, mt.colour);
}


void createWireframe(){
    vector<CanvasTriangle> canvasTriangles; 
    // for each triangle we need to get the canvas triangle first
    // then draw each triangle
    for (std::vector<int>::size_type i = 0; i != triangles.size(); i++){
        CanvasTriangle ct; 
 
        modelToCanvasTri(triangles[i], ct);
        canvasTriangles.push_back(ct);
        //cout << canvasTriangles.back() << endl;
        // cout << i << endl;


        // RASTERISING STARTS HERE
        // with each triangle done we loop through each pixel in the screen
        //drawPoints(canvasTriangles.back());
        drawStrokedTriangle(canvasTriangles.back());

        // assume camera is at origin and cannot be moved yet :))
        // do the fun triangle simialr thing ??

    }

}




// Function for performing animation (shifting artifacts or moving the camera)
void update() {
}


void handleEvent(SDL_Event event, Camera cam) {
  if(event.type == SDL_KEYDOWN) {
    if(event.key.keysym.sym == SDLK_LEFT) cout << "LEFT" << endl;
    else if(event.key.keysym.sym == SDLK_RIGHT) cout << "RIGHT" << endl;
    else if(event.key.keysym.sym == SDLK_UP) cout << "UP" << endl;
    else if(event.key.keysym.sym == SDLK_DOWN) cout << "DOWN" << endl;
    else if(event.key.keysym.sym == SDLK_u){
        // function to create 3 triangle
        // drawRandomTriangle();
        // rand()%255
        cout << "U" << endl;
  
    }
    else if (event.key.keysym.sym == SDLK_f){
        // drawFilledTriangle();
        cout << "F" << endl;
    }
    // if (event.key.keysym.sym == SDLK_a){
    //     cam.cameraPos = cam.cameraRig - (cam.cameraRig * 2.0f);
    //     cout<< "left" <<endl;
    // }
    // if (event.key.keysym.sym == SDLK_d){
    //     cam.cameraPos = cam.cameraRig + (cam.cameraRig * 2.0f);
    //     cout<< "right " <<endl;
    // }
    // if (event.key.keysym.sym == SDLK_w){
    //     cam.cameraPos = cam.cameraFor + (cam.cameraFor * 2.0f);
    //     cout<< "forward " <<endl;
    // }
    // if (event.key.keysym.sym == SDLK_s){
    //     cam.cameraPos = cam.cameraFor - (cam.cameraFor * 2.0f);
    //     cout<< "backward " <<endl;
    // }
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN) cout << "MOUSE CLICKED" << endl;
}
