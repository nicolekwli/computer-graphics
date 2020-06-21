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

// a = from, b = to
vector<CanvasPoint> interpolation(CanvasPoint a, CanvasPoint b, float noOfVals ){
    vector<CanvasPoint> vect;
    float intervalsX;
    float intervalsY;
    CanvasPoint p;

    intervalsX = (b.x - a.x) / (noOfVals);
    intervalsY = (b.y - a.y) / (noOfVals);

    float intervalsTX = (b.texturePoint.x - a.texturePoint.x) / (noOfVals);
    float intervalsTY = (b.texturePoint.y - a.texturePoint.y) / (noOfVals);
    vect.push_back(a);

    for (int i = 1; i < noOfVals; i++) {
        p = CanvasPoint(a.x + intervalsX * i, a.y + intervalsY * i);
        p.texturePoint.x = a.texturePoint.x + intervalsTX * i;
        p.texturePoint.y = a.texturePoint.y + intervalsTY * i;

        vect.push_back(p);
    }
    vect.push_back(b);
    return vect;
}


// ----- Drawing -----
void drawLine(DrawingWindow window, CanvasPoint p1, CanvasPoint p2, Colour c){
  uint32_t colour = bitpackingColour(c);

  float steps = std::max(abs(p1.x - p2.x), abs(p1.y - p2.y));
  vector<CanvasPoint> line = interpolation(p1, p2, steps);

  for (int i=0; i<(int)steps; i++){
    window.setPixelColour((int)line[i].x, (int)line[i].y, colour);
  }
}


void drawStrokedTriangle(DrawingWindow window, CanvasTriangle t){
  uint32_t colour = bitpackingColour(t.colour);

    // sort vertices
    // for (int i = 0; i < 3; i++){
    //     if (t.vertices[2].y < t.vertices[0].y){
    //         swap(t.vertices[2], t.vertices[0]);
    //     }
    //     if (t.vertices[1].y < t.vertices[0].y){
    //         swap(t.vertices[0], t.vertices[1]);
    //     }
    //     if (t.vertices[2].y < t.vertices[1].y){
    //         swap(t.vertices[1], t.vertices[2]);
    //     }
    // }

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

// the swapping bit can be removed - see fill texture
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

    drawStrokedTriangle(window, triangle);
    drawLine(window, newP, triangle.vertices[1],c);

    // Fill top triangle
    float stepsTopLeft = std::max(abs(triangle.vertices[0].x - newP.x), abs(triangle.vertices[0].y - newP.y));
    //vector<CanvasPoint> lineTopLeft = interpolation(triangle.vertices[0], newP, stepsTopLeft);
    vector<CanvasPoint> lineTopLeft = interpolation(triangle.vertices[0], newP, abs(triangle.vertices[0].y - triangle.vertices[1].y)+1);

    float stepsTopRight = std::max(abs(triangle.vertices[0].x - triangle.vertices[1].x), abs(triangle.vertices[0].y - triangle.vertices[1].y));
    //vector<CanvasPoint> lineTopRight = interpolation(triangle.vertices[0], triangle.vertices[1], stepsTopRight);
    vector<CanvasPoint> lineTopRight = interpolation(triangle.vertices[0], triangle.vertices[1], abs(triangle.vertices[0].y - triangle.vertices[1].y)+1);

    //fillTriangle(window, lineTopLeft, lineTopRight, stepsTopLeft, stepsTopRight, c);
    fillTriangle(window, lineTopLeft, lineTopRight, lineTopLeft.size(), lineTopLeft.size(), c);

    // Bottom triangle
    float stepsBottomLeft = std::max(abs(triangle.vertices[2].x - newP.x), abs(triangle.vertices[2].y - newP.y));
    //vector<CanvasPoint> lineBottomLeft = interpolation(newP,triangle.vertices[2], stepsBottomLeft);
    vector<CanvasPoint> lineBottomLeft = interpolation(newP,triangle.vertices[2], abs(triangle.vertices[2].y - triangle.vertices[1].y)+1);

    float stepsBottomRight = std::max(abs(triangle.vertices[2].x - triangle.vertices[1].x), abs(triangle.vertices[2].y - triangle.vertices[1].y));
    //vector<CanvasPoint> lineBottomRight = interpolation(triangle.vertices[1], triangle.vertices[2], stepsBottomRight);
    vector<CanvasPoint> lineBottomRight = interpolation(triangle.vertices[1], triangle.vertices[2], abs(triangle.vertices[2].y - triangle.vertices[1].y)+1);
    
    //fillTriangle(window,lineBottomLeft, lineBottomRight, stepsBottomLeft, stepsBottomRight, c);   
    fillTriangle(window,lineBottomLeft, lineBottomRight, lineBottomLeft.size(), lineBottomLeft.size(), c); 

}


// to/ from
void fillTexture(DrawingWindow window, vector<CanvasPoint> lineTopLeft, vector<CanvasPoint> lineTopRight, vector<vector<uint32_t>> pixels) {
    int steps;
    // make sure we fill according to the longer line so it fills
    for (float a = 0.0; a<lineTopLeft.size(); a++){
        for (float b = 0.0; b<(lineTopRight.size()); b++){
            // for each horizontal line
            if ((int)lineTopLeft[a].y == (int)lineTopRight[b].y){
                steps = (int) abs(lineTopLeft[a].x - lineTopRight[b].x);
                int txsteps = (int) abs(lineTopLeft[a].texturePoint.x - lineTopRight[b].texturePoint.x);
                int tysteps = (int) abs(lineTopLeft[a].texturePoint.y - lineTopRight[b].texturePoint.y);
 
                for (int c = 0; c < steps; c++){
                    int tx = lineTopLeft[a].texturePoint.x + (c * txsteps/steps);
                    int ty = lineTopRight[b].texturePoint.y + (c * tysteps/steps);

                    vector<CanvasPoint> points = interpolation(lineTopLeft[a], lineTopRight[b], steps+1);
                      
                    // if ty = lineTopLeft[a] it will fix top triangle but bottom will be sad
                    //window.setPixelColour((int)lineTopLeft[a].x + c, (int)lineTopRight[b].y, pixels[lineTopLeft[a].texturePoint.y][lineTopLeft[a].texturePoint.x + c]);
                    window.setPixelColour((int)lineTopLeft[a].x + c, (int)lineTopRight[b].y, pixels[points[c].texturePoint.y][points[c].texturePoint.x]);
                    
                    //window.setPixelColour((int)lineTopLeft[a].x + c, (int)lineTopRight[b].y, pixels[ty][tx]);
                }
           }
        }
    }
}


void fillTextureTriangle(DrawingWindow window, vector<vector<uint32_t>> pixels, CanvasTriangle t){
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
    drawStrokedTriangle(window, t);

    // get extra point to split triangle into half
    float steps = std::max(abs(t.vertices[0].x - t.vertices[2].x), abs(t.vertices[0].y - t.vertices[2].y));
    vector<CanvasPoint> line = interpolation(t.vertices[0], t.vertices[2], steps);
    CanvasPoint newP;
    for (int j=0; j<steps; j++){
        if ((int)line[j].y == (int)t.vertices[1].y){
            newP = CanvasPoint(line[j].x, line[j].y);

            float scaleX= (t.vertices[0].x - t.vertices[1].x)/(t.vertices[0].x - t.vertices[2].x);
            float scale = (t.vertices[0].y-t.vertices[1].y)/(t.vertices[0].y-t.vertices[2].y);

            //newP.texturePoint.x = line[j].texturePoint.x;
            //newP.texturePoint.y = t.vertices[1].texturePoint.y;

            newP.texturePoint.x = t.vertices[0].texturePoint.x - scale * (t.vertices[0].texturePoint.x - t.vertices[2].texturePoint.x);
            newP.texturePoint.y = t.vertices[0].texturePoint.y - scale * (t.vertices[0].texturePoint.y - t.vertices[2].texturePoint.y);
        }
    }

    // make sure newP has a smaller value x than vertice 1
    if (newP.x > t.vertices[1].x){
        swap(newP, t.vertices[1]);
    }

    // Fill top triangle
    vector<CanvasPoint> lineTopLeft = interpolation(t.vertices[0], newP, abs(t.vertices[0].y - t.vertices[1].y)+1);
    vector<CanvasPoint> lineTopRight = interpolation(t.vertices[0], t.vertices[1], abs(t.vertices[0].y - t.vertices[1].y)+1);
    fillTexture(window, lineTopLeft, lineTopRight, pixels);

    vector<CanvasPoint> lineBottomLeft = interpolation(t.vertices[2], newP, abs(t.vertices[2].y - t.vertices[1].y)+1);
    vector<CanvasPoint> lineBottomRight = interpolation(t.vertices[2], t.vertices[1], abs(t.vertices[2].y - t.vertices[1].y)+1);
    fillTexture(window,lineBottomLeft, lineBottomRight, pixels); 
}

// ----- Parsing -----
vector<vector<uint32_t>> readPPM(DrawingWindow window, string filename){
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
    vector<uint32_t> pixel;
    vector<vector<uint32_t>> pixels;

    // int i = 0;
    // while (file.get(tempPixel))
    // {
    //   vector<uint32_t> pixel;
    //   if (i == 0){
    //     r = tempPixel;
    //     i++;
    //   } else if (i == 1){
    //     g = tempPixel;
    //     i++;
    //   } else {
    //     b = tempPixel;
    //     uint32_t colour = (255<<24) + (int(r)<<16) + (int(g)<<8) + int(b);
    //     pixel.push_back(colour);
    //     pixels.push_back(pixel);
    //     i = 0;
    //   }
    // }
    for(int y = 0; y < height; y++){
        vector<uint32_t> pixel;
        for (int x = 0; x < width; x++){

        r   = file.get();
        g = file.get();
        b  = file.get();
        uint32_t colour = (255<<24) + (int(r)<<16) + (int(g)<<8) + int(b);
        pixel.push_back(colour);
        }
        pixels.push_back(pixel);
    }

    //display to a window
    // for(int y=height; y>0 ;y--) {
    //     for(int x=width; x>0 ;x--) {
    //         cout << pixels[y][x] << endl;
    //       //window.setPixelColour(x, y, pixels[x][y]);
    //       //pixels.pop_back();
    //     }
    // }

    file.close();
    return pixels;
}