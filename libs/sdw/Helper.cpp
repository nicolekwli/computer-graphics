#include "Helper.h"


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
void drawLine(DrawingWindow window, CanvasPoint p1, CanvasPoint p2){
  float steps = std::max(abs(p1.x - p2.x), abs(p1.y - p2.y));
  vector<CanvasPoint> line = interpolation(p1, p2, steps);
    
  for (int i=0; i<(int)steps; i++){
    uint32_t colour = 255;
    window.setPixelColour((int)line[i].x, (int)line[i].y, colour);
  }
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