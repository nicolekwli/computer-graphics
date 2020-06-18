#include "Helper.h"

// ----- Functions -----
uint32_t bitpackingColour(Colour c){
  uint32_t converted;
  converted = (255<<24) + (c.red<<16) + (c.green<<8) + c.blue;

  return converted;
}

// ----- Interpolation -----
vector<float> interpolation(float from, float to, int noOfVals){
    vector<float> vect;
    float intervals;

    if (from < to){
        intervals = (to - from) / (noOfVals-1);
    } else {
        intervals = (from - to) / (noOfVals-1);
    }

    vect.push_back(from);

    for (int i = 1; i < noOfVals; i++) {
        vect.push_back(from + intervals*i);
    }

    return vect;
}

vector<vec3> interpolation(vec3 from, vec3 to, int noOfVals){
    vector<vec3> vect;
    vec3 intervals;

    intervals = (to - from) / (noOfVals - 1.0f);
    vect.push_back(from);
    
    for (int i = 1; i < noOfVals; i++) {
        vect.push_back(from + intervals*(float)i);
    }

    return vect;
}

vector<CanvasPoint> interpolation(CanvasPoint a, CanvasPoint b, float noOfVals ){
    vector<CanvasPoint> vect;
    float intervalsX;
    float intervalsY;
    CanvasPoint p;
    bool swap = false;
     
    if (a.y > b.y){
        if (a.x < b.x){
            intervalsX = abs(a.x - b.x) / (noOfVals - 1);
            intervalsY = abs(a.y - b.y) / (noOfVals - 1);
        } else {
            swap = true; 
            intervalsX = (a.x - b.x) / (noOfVals - 1);
            intervalsY = (a.y - b.y) / (noOfVals - 1);
        }
    } else {
        if (a.x > b.x){
            swap = true;
            intervalsY = (a.y - b.y) / (noOfVals - 1);
            intervalsX = abs(a.x - b.x) / (noOfVals - 1);
        
        } else {
            intervalsY = abs(a.y - b.y) /(noOfVals - 1);
            intervalsX = abs(a.x - b.x) / (noOfVals - 1);
        }
    }

    vect.push_back(a);

    for (int i = 1; i < noOfVals; i++) {
      if(swap){
        p = CanvasPoint(b.x + intervalsX * i, b.y + intervalsY * i);
      } else{
        p = CanvasPoint(a.x + intervalsX * i, a.y + intervalsY * i);
      }
      vect.push_back(p);
    }
    return vect;
}

// ----- Drawing -----
void drawLine(DrawingWindow window, CanvasPoint p1, CanvasPoint p2, Colour c){
  uint32_t colour = bitpackingColour(c);

  float steps = std::max(abs(p1.x - p2.x), abs(p1.y - p2.y));
  vector<CanvasPoint> line = interpolation(p1, p2, steps);

  for (int i=0; i<(int)steps; i++){
    // uint32_t colour = 255;
    cout << line[i].x << endl;
    window.setPixelColour((int)line[i].x, (int)line[i].y, colour);
  }
}

void drawStrokedTriangle(DrawingWindow window, CanvasTriangle t){
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
    vector<CanvasPoint> lineAB = interpolation(t.vertices[0], t.vertices[1], stepsAB);
   
    float stepsBC = std::max(abs(t.vertices[1].x - t.vertices[2].x), abs(t.vertices[1].y - t.vertices[2].y));
    vector<CanvasPoint> lineBC = interpolation(t.vertices[1], t.vertices[2], stepsBC);
   
    float stepsAC = std::max(abs(t.vertices[0].x - t.vertices[2].x), abs(t.vertices[0].y - t.vertices[2].y));
    vector<CanvasPoint> lineAC = interpolation(t.vertices[0], t.vertices[2], stepsAC);
    
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

// should use the prev functions
void fillTriangle(DrawingWindow window, vector<CanvasPoint> lineTopLeft, vector<CanvasPoint> lineTopRight, float stepsTopLeft, float stepsTopRight, Colour c) {
    uint32_t colour = bitpackingColour(c);
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
    // make sure we fill according to the longer line so it fills
    for (float a = 0.0; a<std::max(stepsTopLeft, stepsTopRight); a++){
        // width of line 
        // draw a horizontal line
        for (float b = 0.0; b<(widthLine.size()); b++){
            if ((int)heightLine[a].y == (int)widthLine[b].y){
                width = abs(heightLine[a].x - widthLine[b].x);
                for (float c = 0.0; c < width; c++){
                    if (swap){
                        window.setPixelColour((int)heightLine[a].x - (int)c, (int)widthLine[b].y, colour);
                    } else {
                        window.setPixelColour((int)heightLine[a].x + (int)c, (int)widthLine[b].y, colour);
                    }
                }
            }
        }
    }
}

void drawFilledTriangle(DrawingWindow window, Colour c){
    CanvasTriangle triangle = CanvasTriangle(CanvasPoint(rand()%200, rand()%150), 
                                             CanvasPoint(rand()%200, rand()%150),
                                             CanvasPoint(rand()%200, rand()%150),
                                             c );
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
    
    drawStrokedTriangle(window, triangle);

    // get extra point to split triangle into half
    float steps = std::max(abs(triangle.vertices[0].x - triangle.vertices[2].x), abs(triangle.vertices[0].y - triangle.vertices[2].y));
    vector<CanvasPoint> line = interpolation(triangle.vertices[0], triangle.vertices[2], steps);
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
    vector<CanvasPoint> lineTopLeft = interpolation(triangle.vertices[0], newP, stepsTopLeft);
    float stepsTopRight = std::max(abs(triangle.vertices[0].x - triangle.vertices[1].x), abs(triangle.vertices[0].y - triangle.vertices[1].y));
    vector<CanvasPoint> lineTopRight = interpolation(triangle.vertices[0], triangle.vertices[1], stepsTopRight);
    fillTriangle(window, lineTopLeft, lineTopRight, stepsTopLeft, stepsTopRight, c);

    // Bottom triangle
    float stepsBottomLeft = std::max(abs(triangle.vertices[2].x - newP.x), abs(triangle.vertices[2].y - newP.y));
    vector<CanvasPoint> lineBottomLeft = interpolation(newP,triangle.vertices[2], stepsBottomLeft);
    float stepsBottomRight = std::max(abs(triangle.vertices[2].x - triangle.vertices[1].x), abs(triangle.vertices[2].y - triangle.vertices[1].y));
    vector<CanvasPoint> lineBottomRight = interpolation(triangle.vertices[1], triangle.vertices[2], stepsBottomRight);
    fillTriangle(window,lineBottomLeft, lineBottomRight, stepsBottomLeft, stepsBottomRight, c);   

    //drawLine(window, newP, triangle.vertices[1], Colour(255, 255, 255));
}

// ----- Parsing -----
 vector<uint32_t> readPPM(string filename){
    ifstream file;
    file.open(filename);

    string ptype;
    string line;
    int width = 0, height = 0, max = 0;

    //TOD0: if line starts with # ignore it
    // get P type, height, width and max
    getline(file, ptype);
    getline(file, line);
    getline(file, line);

    size_t found = line.find(' ');
    width = stoi(line.substr(0, found));

    size_t found2 = line.find(' ', found + 1);
    height = stoi(line.substr(found + 1, found2 - 1));

    getline(file, line);
    max = stoi(line);
    
    char tempPixel;
    char r,g,b;
    vector<uint32_t> pixels;

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
    // for(int y=height; y>0 ;y--) {
    //     for(int x=width; x>0 ;x--) {
    //       //window.setPixelColour(x, y, pixels.back());
    //       pixels.pop_back();
    //     }
    // }

    file.close();
    return pixels;
}