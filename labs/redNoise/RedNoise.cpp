#include <ModelTriangle.h>
#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;
using namespace glm;

// 320/240
// #define WIDTH 1419
// #define HEIGHT 1001
#define WIDTH 500
#define HEIGHT 400

void draw();
void update();
vector<float> interpolation(float from, float to, int noOfVal);
vector<vec3> interpolationV(vec3 from, vec3 to, int noOfVal);
vector<CanvasPoint> interpolationCanvasPoints(CanvasPoint a, CanvasPoint b, float noOfVal );
void handleEvent(SDL_Event event);
void drawGreyscale();
void drawColour();
void drawLine(CanvasPoint p1, CanvasPoint p2);
void drawStrokedTriangle(CanvasTriangle t);
void drawRandomTriangle();
uint32_t bitpackingColour(Colour c);
void drawFilledTriangle();
void fillTriangle(vector<CanvasPoint> lineTopLeft, vector<CanvasPoint> lineTopRight, float stepsTopLeft, float stepsTopRight); 
void readPPM(string filename);
void getTexture(CanvasTriangle t);
void textureMapFill(vector<CanvasPoint> lineTopLeft, vector<CanvasPoint> lineTopRight, float stepsTopLeft, float stepsTopRight, CanvasTriangle t);
void textureMap();


DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
vector<uint32_t> pixels;
vector<uint32_t> texturePixels;
int widthT = -1;
int heightT = -1;


int main(int argc, char* argv[]){
    SDL_Event event;

    //Lab1: task4
    //vector<float> res = interpolation( 2.2, 8.5, 7 );
    //vec3 from( 1, 4, 9.2 );
    //vec3 to( 4, 1, 9.8 );
    //vector<vec3> re = interpolationV(from, to, 4);

    //Lab2: task2
    /*CanvasTriangle triag = CanvasTriangle(CanvasPoint(40,100),
                                          CanvasPoint(80,200),
                                          CanvasPoint(20,160),
                                          Colour(0, 255, 0));
    */
  while(true) {
    
    // We MUST poll for events - otherwise the window will freeze !
    if(window.pollForInputEvents(&event)) handleEvent(event);
    update();
    //draw();
    // drawLine(40,100,80,200);
    //drawStrokedTriangles(a,b,c);
    //drawFilledTriangle();

    // drawGreyscale();
    // drawLine(CanvasPoint(100,20), CanvasPoint(20,200));
    // drawStrokedTriangle(triag);
    //drawFilledTriangle();
    //readPPM("texture.ppm");
    //textureMap();

    // Need to render the frame at the end, or nothing actually gets shown on the screen !
    window.renderFrame();
  }
 }
 

void draw(){
  window.clearPixels();
  for(int y=0; y<window.height ;y++) {
    for(int x=0; x<window.width ;x++) {
      float red = rand() % 255;
      float green = 0.0;
      float blue = 0.0;
      uint32_t colour = (255<<24) + (int(red)<<16) + (int(green)<<8) + int(blue);
      window.setPixelColour(x, y, colour);
    }
  }
}


// Function for performing animation (shifting artifacts or moving the camera)
void update() {
}


// ---- interpolation
vector<float> interpolation(float from, float to, int noOfVal ){
    vector< float > vect;
    float intervals;

    if (from < to){
        intervals = (to - from) / (noOfVal-1);
    
    } else {
        intervals = (from - to) / (noOfVal-1);
    }
    vect.push_back(from);

    for (int i=1; i<noOfVal; i++) {
        vect.push_back(from + intervals*i);
    }
    return vect;
}

//---Lab 1: task 3
void drawGreyscale(){
  window.clearPixels();
  
  vector<float> greyInt = interpolation(255, 0, WIDTH);
  
  for(int y=0; y<window.height ;y++) {
    for(int x=0; x<window.width ;x++) {
      float red = 0.0 + greyInt[x];
      float green = 0.0 + greyInt[x];
      float blue = 0.0 + greyInt[x];
      uint32_t colour = (255<<24) + (int(red)<<16) + (int(green)<<8) + int(blue);
      window.setPixelColour(x, y, colour);
      }
  }
}


//---Lab 1: task 4
vector<vec3> interpolationV(vec3 from, vec3 to, int noOfVal ){
  vector<vec3> vect;
  vec3 intervals;
  intervals = (to - from) / (noOfVal - 1.0f); // :o
  vect.push_back(from);
  
  for (int i=1; i<noOfVal; i++) {
    vect.push_back(from + intervals*(float)i);
  }

  return vect;
}

//---Lab 1: task5
void drawColour(){
     window.clearPixels();

     vec3 helloR = vec3(255,0,0);
     vec3 helloB = vec3(0,0,255);
     vec3 helloG = vec3(0,255,0);
     vec3 helloY = vec3(255,255,0);

     vector<vec3> right = interpolationV(helloB, helloG, HEIGHT); 
     vector<vec3> greyInt = interpolationV(helloR, helloY, HEIGHT);
        
     for(int y=0; y<window.height ;y++) {
      vector<vec3> line = interpolationV(greyInt[y], right[y], WIDTH); 
      for(int x=0; x<window.width ;x++) {
         float red =  line[x].r;
         float green =  line[x].g;
         float blue = line[x].b;
         uint32_t colour = (255<<24) + (int(red)<<16) + (int(green)<<8) + int(blue);
         window.setPixelColour(x, y, colour);
        }
     }
 }

//---Lab2: task1
void drawLine(CanvasPoint p1, CanvasPoint p2){
  float steps = std::max(abs(p1.x - p2.x), abs(p1.y - p2.y));
  vector<CanvasPoint> line = interpolationCanvasPoints(p1, p2, steps);
    
  for (int i=0; i<(int)steps; i++){
    uint32_t colour = 255;
    window.setPixelColour((int)line[i].x, (int)line[i].y, colour);
  }
}

//--- Lab2: task2
vector<CanvasPoint> interpolationCanvasPoints(CanvasPoint a, CanvasPoint b, float noOfVal ){
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


//--- Lab2: task2
uint32_t bitpackingColour(Colour c){
  uint32_t converted;
  converted = (255<<24) + (c.red<<16) + (c.green<<8) + c.blue;

  return converted;
}


void drawStrokedTriangle(CanvasTriangle t){
  uint32_t colour = bitpackingColour(t.colour);

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


void drawRandomTriangle(){
  CanvasTriangle t = CanvasTriangle(CanvasPoint(rand()%300, rand()%150), CanvasPoint(rand()%300, rand()%150), CanvasPoint(rand()%300, rand()%150));
  drawStrokedTriangle(t);
}


void fillTriangle(vector<CanvasPoint> lineTopLeft, vector<CanvasPoint> lineTopRight, float stepsTopLeft, float stepsTopRight) {
    // Start fill top triangle
    vector<CanvasPoint> widthLine;
    vector<CanvasPoint> heightLine;
    bool swap = false; 

    // Code was written according to the triangle in his slides
    // would not work if its got dofferent lengths to his triangle
    // wrote some dodgey swaping when it isn't so it works
    if (std::max(stepsTopLeft, stepsTopRight) == stepsTopLeft){
        widthLine = lineTopRight;
        heightLine = lineTopLeft;
    } else {
        widthLine = lineTopLeft;
        heightLine = lineTopRight;
        swap = true;
    }
    // float height = abs(triangle.vertices[0].y - triangle.vertices[1].y);
    float width;
    // make sure we fill according to the longer line so it fills
    for (float a = 0.0; a<std::max(stepsTopLeft, stepsTopRight); a++){
        // width of line 
        // draw a horizontal line
        for (float b = 0.0; b<(widthLine.size()); b++){
            if ((int)heightLine[a].y == (int)widthLine[b].y){
                width = abs(heightLine[a].x - widthLine[b].x);
                for (float c = 0.0; c < width; c++){
                    if (swap){
                        window.setPixelColour((int)heightLine[a].x - (int)c, (int)widthLine[b].y, 12345);
                    } else {
                        window.setPixelColour((int)heightLine[a].x + (int)c, (int)widthLine[b].y, 12345);
                    }
                }
            }
        }
    }
}

//--- Lab2: task3
void drawFilledTriangle(){
    CanvasTriangle triangle = CanvasTriangle(CanvasPoint(rand()%200, rand()%150), 
                                             CanvasPoint(rand()%200, rand()%150),
                                             CanvasPoint(rand()%200, rand()%150),
                                             Colour(255, 255, 255) );
    // sort vertices
    // might not need as is already in drawStrokedTriangle
    for (int i = 0; i < 3; i++){
        if (triangle.vertices[2].y < triangle.vertices[0].y){
            swap(triangle.vertices[2], triangle.vertices[0]);
        }
        if (triangle.vertices[1].y < triangle.vertices[0].y){
            swap(triangle.vertices[0], triangle.vertices[1]);
        }
        if (triangle.vertices[2].y < triangle.vertices[1].y){
            swap(triangle.vertices[1], triangle.vertices[2]);
        }
    }
    
    drawStrokedTriangle(triangle);

    // get extra point to split triangle into half
    float steps = std::max(abs(triangle.vertices[0].x - triangle.vertices[2].x), abs(triangle.vertices[0].y - triangle.vertices[2].y));
    vector<CanvasPoint> line = interpolationCanvasPoints(triangle.vertices[0], triangle.vertices[2], steps);
    CanvasPoint newP;
    for (int j=0; j<steps; j++){
        if ((int)line[j].y == (int)triangle.vertices[1].y){
            newP = CanvasPoint(line[j].x, line[j].y);
        }
    }

    // make sure newP has a smaller value x than vertice 1
    if (newP.x > triangle.vertices[1].x){
        swap(newP, triangle.vertices[1]);
    }

    // Fill top triangle
    float stepsTopLeft = std::max(abs(triangle.vertices[0].x - newP.x), abs(triangle.vertices[0].y - newP.y));
    vector<CanvasPoint> lineTopLeft = interpolationCanvasPoints(triangle.vertices[0], newP, stepsTopLeft);
    float stepsTopRight = std::max(abs(triangle.vertices[0].x - triangle.vertices[1].x), abs(triangle.vertices[0].y - triangle.vertices[1].y));
    vector<CanvasPoint> lineTopRight = interpolationCanvasPoints(triangle.vertices[0], triangle.vertices[1], stepsTopRight);
    fillTriangle(lineTopLeft, lineTopRight, stepsTopLeft, stepsTopRight);

    // Bottom triangle
    float stepsBottomLeft = std::max(abs(triangle.vertices[2].x - newP.x), abs(triangle.vertices[2].y - newP.y));
    vector<CanvasPoint> lineBottomLeft = interpolationCanvasPoints(newP,triangle.vertices[2], stepsBottomLeft);
    float stepsBottomRight = std::max(abs(triangle.vertices[2].x - triangle.vertices[1].x), abs(triangle.vertices[2].y - triangle.vertices[1].y));
    vector<CanvasPoint> lineBottomRight = interpolationCanvasPoints(triangle.vertices[1], triangle.vertices[2], stepsBottomRight);
    fillTriangle(lineBottomLeft, lineBottomRight, stepsBottomLeft, stepsBottomRight);   
}


// void drawRandomFilledTriangle(){
//   for (int i=0; i<3; i++ ){
//       CanvasTriangle t = CanvasTriangle(CanvasPoint(rand()%300, rand()%150), CanvasPoint(rand()%300, rand()%150), CanvasPoint(rand()%300, rand()%150));

//       drawFilledTriangle(t);
//   }
// }


struct pixel {
     char red;
     char green;
     char blue;
};

// --- LAB 2: TASK 4
/* 
You should parse in the PPM image file in your own code.
Once the pixel data has been loaded, show the image in the display window.
Your code should set each pixel of the window individually  
*/
void readPPM(string filename){
    ifstream file;
    file.open(filename);

    // if(substr()isNULL) { //file doesnt exist causes error
    //     cout<<"FILE DOESNT EXIST");
    //     exit(0);
    // }

    string ptype;
    string line;
    int width = 0, height = 0, max = 0;

    //TOD0: if line starts with # ignore it

    // get P type, height, width and max
    getline(file, ptype);
    getline(file, line);
    getline(file, line);

    string str (" ");
    size_t found = line.find(str);
    string w = line.substr(0, found);

    size_t found2 = line.find(str, found+1);
    string h = line.substr(found+1, found2-1);

    height = stoi(h);
    heightT = height;
    width = stoi(w);
    widthT = width;

    getline(file, line);
    max = stoi(line);

    // if(ptype.compare("P6")) != 0){ //Make sure we have a P6 file
    //     cout<<"Type is not P6 !!!";
    // }
    
    // pixel p[width/3]; //rip how many pixels exist
    vector<char> pixelArray;
    char tempPixel;
    char r,g,b;
    //vector<uint32_t> pixels;

    int i = 0;
    while (file.get(tempPixel))
    {
      if (i == 0){
        r = tempPixel;
        i++;
      } else if (i == 1){
        g = tempPixel;
        i++;
      } else {
        b = tempPixel;
        uint32_t colour = (255<<24) + (int(r)<<16) + (int(g)<<8) + int(b);
        pixels.push_back(colour);
        i = 0;
      }
    }
    // display to a window
    for(int y=height; y>0 ;y--) {
        for(int x=width; x>0 ;x--) {
          window.setPixelColour(x, y, pixels.back());
         // pixels.pop_back();
        }
    }

    file.close();
}

// task 5 - get the texture section
/* 
* pixels is the name for the variable that has then entire ppm
* texturePixels is the vector with all the needed pixels
*/
void getTexture(CanvasTriangle t){
  // get extra point for top triangle
  float stepsTexture = std::max(abs(t.vertices[0].texturePoint.x - t.vertices[2].texturePoint.x), abs(t.vertices[0].texturePoint.y - t.vertices[2].texturePoint.y));
  CanvasPoint tempT = CanvasPoint(t.vertices[0].texturePoint.x, t.vertices[0].texturePoint.y);
  CanvasPoint tempT1 = CanvasPoint(t.vertices[1].texturePoint.x, t.vertices[1].texturePoint.y);
  CanvasPoint tempT2 = CanvasPoint(t.vertices[2].texturePoint.x, t.vertices[2].texturePoint.y);
  
  vector<CanvasPoint> lineT = interpolationCanvasPoints(tempT, tempT2, stepsTexture);

  CanvasPoint newPT;
  for (int j=0; j<stepsTexture; j++){
    if ((int)lineT[j].y == (int)t.vertices[1].y){
        newPT = CanvasPoint(lineT[j].x, lineT[j].y);
    }
  }

  float stepsTopLeft = std::max(abs(tempT.x - newPT.x), abs(tempT.y - newPT.y));
  vector<CanvasPoint> lineTopLeft = interpolationCanvasPoints(tempT, newPT, stepsTopLeft);
  float stepsTopRight = std::max(abs(tempT.x - tempT1.x), abs(tempT.y - tempT1.y));
  vector<CanvasPoint> lineTopRight = interpolationCanvasPoints(tempT, tempT1, stepsTopRight);

  // create vector of texture points
  vector<CanvasPoint> texturePoints;
  CanvasPoint p;
  //float steps;

    // -------------------------------------------- !!!! TODO !!!
  // THIS BIT NEEDS MODIFYING ----- everything with filling the triangle and stuff is done just getting the texture that isn't working
  // This whole lot of faff should give us a list of points we take from the pixelmap
  // NEED TO INTERPOLATE BETWEEN EACH POINT ON LEFT LINE WITH RIGHT
  // THEN GET THE CORRESPONDING PIXELS...?
  // SInce we will be dealing with floats
  // There will be overlapped rounding ups/downs
  // Which causes the image to stretch
  vector<CanvasPoint> widthLine;
  vector<CanvasPoint> heightLine;
  bool swap = false; 
  if (std::max(stepsTopLeft, stepsTopRight) == stepsTopLeft){
      widthLine = lineTopRight;
      heightLine = lineTopLeft;
  } else {
      widthLine = lineTopLeft;
      heightLine = lineTopRight;
      swap = true;
  }
  
  float width;
  for (float a = 0.0; a<std::max(stepsTopLeft, stepsTopRight); a++){
      // width of line 
      // draw a horizontal line
      for (float b = 0.0; b<(widthLine.size()); b++){
          if ((int)heightLine[a].y == (int)widthLine[b].y){
              width = abs(heightLine[a].x - widthLine[b].x);
              for (float c = 0.0; c < width; c++){
                if (swap){
                      p = CanvasPoint((int)heightLine[a].x - (int)c, (int)widthLine[b].y);
                      // window.setPixelColour((int)heightLine[a].x - (int)c, (int)widthLine[b].y, texturePixels.back());
                      texturePoints.push_back(p);
                  } else {
                      p = CanvasPoint((int)heightLine[a].x + (int)c, (int)widthLine[b].y);
                      texturePoints.push_back(p);
                  }
              }
          }
      }
  }

  for (int i=0; i<texturePoints.size(); i++){
    for(int y=heightT; y>0 ;y--) {
        for(int x=widthT; x>0 ;x--) {
 
          if ((y == texturePoints[i].y) && (x == texturePoints[i].x)){
            texturePixels.push_back(pixels.back());
          } 

        }
    }
  }

  /*for (int i=0; i< (newPT.y - t.vertices[0].texturePoint.x); i++){
    steps = std::max(abs(lineTopLeft[i].x - lineTopRight[i].x), abs(lineTopLeft[i].y - lineTopRight[i].y));
    texturePoints = interpolationCanvasPoints(lineTopLeft[i], lineTopRight[i], steps);
    
    //cout << texturePoints.size() << endl;
    for (int i=0; i<texturePoints.size(); i++){
      for(int y=heightT; y>0 ;y--) {
        for(int x=widthT; x>0 ;x--) {
          
          if ((y == texturePoints[i].y) && (x == texturePoints[i].x)){
            texturePixels.push_back(pixels.back());
            cout << "HERE" << endl;
          } 
          //pixels.pop_back();
          
          //texturePixels.push_back();
        }
      }
    }
  }*/
    
} // end of getTexture()

// task 5 - texturemap filling triangle bit
void textureMapFill(vector<CanvasPoint> lineTopLeft, vector<CanvasPoint> lineTopRight, float stepsTopLeft, float stepsTopRight, CanvasTriangle t){
  // Start fill top triangle
  vector<CanvasPoint> widthLine;
  vector<CanvasPoint> heightLine;
  bool swap = false; 
  if (std::max(stepsTopLeft, stepsTopRight) == stepsTopLeft){
      widthLine = lineTopRight;
      heightLine = lineTopLeft;
  } else {
      widthLine = lineTopLeft;
      heightLine = lineTopRight;
      swap = true;
  }
  // float height = abs(triangle.vertices[0].y - triangle.vertices[1].y);
  float width;
  for (float a = 0.0; a<std::max(stepsTopLeft, stepsTopRight); a++){
      // width of line 
      // draw a horizontal line
      for (float b = 0.0; b<(widthLine.size()); b++){
          if ((int)heightLine[a].y == (int)widthLine[b].y){
              width = abs(heightLine[a].x - widthLine[b].x);
              for (float c = 0.0; c < width; c++){

                  if (swap){
                      window.setPixelColour((int)heightLine[a].x - (int)c, (int)widthLine[b].y, 222);
                  } else {
                      window.setPixelColour((int)heightLine[a].x + (int)c, (int)widthLine[b].y, 222);
                  }
                  //texturePixels.pop_back();
              }
          }
      }
  }

}

// task 5
void textureMap(){
  // draw stroked triangle
  CanvasPoint a = CanvasPoint(160,10);
  CanvasPoint b = CanvasPoint(300, 230);
  CanvasPoint c = CanvasPoint(10, 150);

  CanvasTriangle t = CanvasTriangle(a, b, c);

  t.vertices[0].texturePoint.x = 195;
  t.vertices[0].texturePoint.y = 5;
  t.vertices[1].texturePoint.x = 395;
  t.vertices[1].texturePoint.y = 380;
  t.vertices[2].texturePoint.x = 65;
  t.vertices[2].texturePoint.y = 330;

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

  //   drawStrokedTriangle(t);

  //-- display the texture triangle points
  CanvasPoint at = CanvasPoint(195,5);
  CanvasPoint bt = CanvasPoint(395, 380);
  CanvasPoint ct = CanvasPoint(65, 330);
  CanvasTriangle textTriag = CanvasTriangle(at, bt, ct, Colour(255, 0, 0));
  // drawStrokedTriangle(textTriag);

  getTexture(t);

  // copy code
  // get extra point for triangle
  float steps = std::max(abs(t.vertices[0].x - t.vertices[2].x), abs(t.vertices[0].y - t.vertices[2].y));
  vector<CanvasPoint> line = interpolationCanvasPoints(t.vertices[0], t.vertices[2], steps);
  CanvasPoint newP;
  for (int j=0; j<steps; j++){
      if ((int)line[j].y == (int)t.vertices[1].y){
          newP = CanvasPoint(line[j].x, line[j].y);
      }
  }

  // make sure newP has a smaller value x than vertice 1
  if (newP.x > t.vertices[1].x){
      swap(newP, t.vertices[1]);
    }

  // Fill top triangle
  float stepsTopLeft = std::max(abs(t.vertices[0].x - newP.x), abs(t.vertices[0].y - newP.y));
  vector<CanvasPoint> lineTopLeft = interpolationCanvasPoints(t.vertices[0], newP, stepsTopLeft);
  float stepsTopRight = std::max(abs(t.vertices[0].x - t.vertices[1].x), abs(t.vertices[0].y - t.vertices[1].y));
  vector<CanvasPoint> lineTopRight = interpolationCanvasPoints(t.vertices[0], t.vertices[1], stepsTopRight);
  textureMapFill(lineTopLeft, lineTopRight, stepsTopLeft, stepsTopRight, t);
  // rasterise and fill triangle with variable pixels

  // Fill bottom????
}



void handleEvent(SDL_Event event)
{
  if(event.type == SDL_KEYDOWN) {
    if(event.key.keysym.sym == SDLK_LEFT) cout << "LEFT" << endl;
    else if(event.key.keysym.sym == SDLK_RIGHT) cout << "RIGHT" << endl;
    else if(event.key.keysym.sym == SDLK_UP) cout << "UP" << endl;
    else if(event.key.keysym.sym == SDLK_DOWN) cout << "DOWN" << endl;
    else if(event.key.keysym.sym == SDLK_u){
        drawRandomTriangle();
        cout << "U" << endl;
    }
    else if (event.key.keysym.sym == SDLK_f){
        drawFilledTriangle();
    }    
    else if(event.key.keysym.sym == SDLK_f){
     // drawRandomFilledTriangle();
    } 
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN) cout << "MOUSE CLICKED" << endl;
}
