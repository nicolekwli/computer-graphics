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
    float intervalsDepth = (b.depth - a.depth) / (noOfVals);
    vect.push_back(a);

    for (int i = 1; i < noOfVals; i++) {
        // double check depth
        p = CanvasPoint(a.x + intervalsX * i, a.y + intervalsY * i, a.depth + intervalsDepth * i) ;
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
  vector<CanvasPoint> line = interpolation(p2, p1, steps);

  for (int i=0; i<(int)steps+1; i++){
    window.setPixelColour((int)line[i].x, (int)line[i].y, line[i].depth, colour);
    //window.setPixelColour((int)line[i].x, (int)line[i].y, colour);
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

    drawLine(window, t.vertices[0], t.vertices[1], t.colour);
    drawLine(window, t.vertices[0], t.vertices[2], t.colour);
    drawLine(window, t.vertices[1], t.vertices[2], t.colour);
}

//** HAVE CUT DOWN CODE
// the swapping bit can be removed - see fill texture
void fillTriangle(DrawingWindow window, vector<CanvasPoint> lineTopLeft, vector<CanvasPoint> lineTopRight, Colour c) {
    uint32_t colour = bitpackingColour(c);

    // make sure we fill according to the longer line so it fills
    for (float a = 0.0; a<lineTopLeft.size(); a++){
        // ** EITHER THIS LINE
        drawLine(window, lineTopLeft[a], lineTopRight[a], c);

        // ** OR THIS FOR LOOP WOULD DO THE SAME THING
        // width = (int) abs(lineTopLeft[a].x - lineTopRight[a].x);
        // for (float c = 0; c <= width; c++){
        //     //window.setPixelColour((int)lineTopLeft[a].x + c, (int)lineTopRight[b].y, colour);
        //     window.setPixelColour((int)lineTopLeft[a].x + c, (int)lineTopLeft[a].y, lineTopRight[a].depth ,colour);
        // }

    }
}

void drawFilledTriangle(DrawingWindow window, Colour c, CanvasTriangle triangle){
    // CanvasTriangle triangle = CanvasTriangle(CanvasPoint(rand()%200, rand()%150), 
    //                                          CanvasPoint(rand()%200, rand()%150),
    //                                          CanvasPoint(rand()%200, rand()%150),
    //                                          c );
    // sort vertices, [0] is at the topm [1] middle [2] bottom
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

    float ratio = (triangle.vertices[1].y - triangle.vertices[0].y) / (triangle.vertices[2].y - triangle.vertices[0].y);
    //float ratioR = (triangle.vertices[1].depth - triangle.vertices[0].depth) / (triangle.vertices[2].depth - triangle.vertices[0].depth);
    CanvasPoint newP = CanvasPoint(triangle.vertices[0].x + ratio * (triangle.vertices[2].x - triangle.vertices[0].x), 
                                        triangle.vertices[0].y + ratio * (triangle.vertices[2].y - triangle.vertices[0].y),
                                        triangle.vertices[0].depth + ratio * (triangle.vertices[2].depth - triangle.vertices[0].depth));


    // make sure newP has a smaller value x than vertice 1
    if (newP.x > triangle.vertices[1].x){
        swap(newP, triangle.vertices[1]);
    }

    // ** THIS WORKS
    drawStrokedTriangle(window, triangle);
    drawLine(window, newP, triangle.vertices[1],c);
    drawLine(window, newP, triangle.vertices[2],c);
    drawLine(window, triangle.vertices[0], triangle.vertices[1],c);
    drawLine(window, triangle.vertices[1], triangle.vertices[2],c);

    // ** PLS CHECK IF THIS GETS THE RIGHT POINTS
    // Fill top triangle
    vector<CanvasPoint> lineTopLeft = interpolation(triangle.vertices[0], newP, abs(triangle.vertices[0].y - triangle.vertices[1].y)+1);
    vector<CanvasPoint> lineTopRight = interpolation(triangle.vertices[0], triangle.vertices[1], abs(triangle.vertices[0].y - triangle.vertices[1].y)+1);
    fillTriangle(window, lineTopLeft, lineTopRight, triangle.colour);

    // Bottom triangle
    vector<CanvasPoint> lineBottomLeft = interpolation(newP,triangle.vertices[2], abs(triangle.vertices[2].y - triangle.vertices[1].y)+1);
    vector<CanvasPoint> lineBottomRight = interpolation(triangle.vertices[1], triangle.vertices[2], abs(triangle.vertices[2].y - triangle.vertices[1].y)+1);
    fillTriangle(window,lineBottomLeft, lineBottomRight, triangle.colour); 

}



// to/ from
void fillTexture(DrawingWindow window, vector<CanvasPoint> lineTopLeft, vector<CanvasPoint> lineTopRight, vector<vector<uint32_t>> pixels) {
    int steps;
    // make sure we fill according to the longer line so it fills
    for (float a = 0.0; a<lineTopLeft.size(); a++){
        steps = (int) abs(lineTopLeft[a].x - lineTopRight[a].x);
        vector<CanvasPoint> points = interpolation(lineTopLeft[a], lineTopRight[a], steps+1);
        // drawLine(lineTopLeft[a], lineTopRight, pixels[points[c].texturePoint.y][points[c].texturePoint.x]);

        for (int c = 0; c < steps; c++){
            // Add bit that does shading/ illumination in carl's
            // depth buffer is already a kind of shading?
            window.setPixelColour((int)lineTopLeft[a].x + c, (int)lineTopRight[a].y, pixels[points[c].texturePoint.y][points[c].texturePoint.x]);
        }
    }
}


//  for logo.obj each texture value is 0.0-1.0, need to convert to the ppm size
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
    //drawStrokedTriangle(window, t);
    //drawFilledTriangle(window, Colour(0,0,0), t);

    float ratio = (t.vertices[1].y - t.vertices[0].y) / (t.vertices[2].y - t.vertices[0].y);

    CanvasPoint newP = CanvasPoint(t.vertices[0].x + ratio * (t.vertices[2].x - t.vertices[0].x), 
                                        t.vertices[0].y + ratio * (t.vertices[2].y - t.vertices[0].y),
                                        t.vertices[0].depth + ratio * (t.vertices[2].depth) - t.vertices[0].depth);

    float scale = (t.vertices[0].y-t.vertices[1].y)/(t.vertices[0].y-t.vertices[2].y);
    newP.texturePoint.x = t.vertices[0].texturePoint.x - scale * (t.vertices[0].texturePoint.x - t.vertices[2].texturePoint.x);
    newP.texturePoint.y = t.vertices[0].texturePoint.y - scale * (t.vertices[0].texturePoint.y - t.vertices[2].texturePoint.y);

    // make sure newP has a smaller value x than vertice 1
    if (newP.x > t.vertices[1].x){
        swap(newP, t.vertices[1]);
    }

    // // Fill top triangle
    vector<CanvasPoint> lineTopLeft = interpolation(t.vertices[0], newP, abs(t.vertices[0].y - t.vertices[1].y)+1);
    vector<CanvasPoint> lineTopRight = interpolation(t.vertices[0], t.vertices[1], abs(t.vertices[0].y - t.vertices[1].y)+1);
    fillTexture(window, lineTopLeft, lineTopRight, pixels);

    vector<CanvasPoint> lineBottomLeft = interpolation(t.vertices[2], newP, abs(t.vertices[2].y - t.vertices[1].y)+1);
    vector<CanvasPoint> lineBottomRight = interpolation(t.vertices[2], t.vertices[1], abs(t.vertices[2].y - t.vertices[1].y)+1);
    fillTexture(window,lineBottomLeft, lineBottomRight, pixels); 
}

// ----- Drawing Helpers -----

// ----- Parsing -----
PPM readPPM(DrawingWindow window, string filename){
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
    //         //cout << pixels[y][x] << endl;
    //       window.setPixelColour(x, y, pixels[x][y]);
    //       //pixels.pop_back();
    //     }
    // }

    file.close();

    PPM ppm = PPM(width, height, pixels);

    return ppm;
}

void savePPM(DrawingWindow window, string filename){
    ofstream file;
    file.open(filename, ios::out | ios::binary);
    if (!file) {
        cerr << "Cannot open file" << endl;
        //return false;
    }
    file << "P6" << endl;
    file << window.width << "\n";
    file << window.height << "\n";
    file << "255" << "\n";


    for (int x = 0; x < window.height ; x++) {

        for (int y = 0; y < window.width ; y++){
            uint32_t p = window.getPixelColour(y,x);
            Colour c = Colour((p & 0x00ff0000) >> 16, (p & 0x0000ff00) >> 8, p & 0x000000ff);

            file.write((const char*)(&c.red),1 );
            file.write((const char*)(&c.green),1 );
            file.write((const char*)(&c.blue),1 );
        }
    }

    if (file.fail()) {
        cerr << "Could not write data" << endl;
        //return false;
    }

    file.close();
}


vector<ModelTriangle> readOBJ(string filename, vector<Colour> colours, PPM ppm, float rescale){
    ifstream file;
    file.open(filename);

    string line;
    string *tokens;
    Colour col;
    string objectName;

    vector<ModelTriangle> triangles;
    vector<glm::vec3> ver;
    vector<TexturePoint> verTexture;

    // get first line matllib
    getline(file, line);
    
    while (getline(file, line)){
        tokens = split(line, ' ');

        if (tokens[0] == "o"){
            objectName = tokens[1];

        }
        else if (tokens[0] == "usemtl") {
            // Colour
            for (std::vector<int>::size_type i = 0; i != colours.size(); i++){
                if(colours[i].name == tokens[1]){
                    col = colours[i];
                }
            }
        } 
        else if (tokens[0] == "v"){
            // * each by a scale to make smaller if need
            glm::vec3 vec = vec3(stof(tokens[1])*rescale, stof(tokens[2])*rescale, stof(tokens[3])*-rescale);
            ver.push_back(vec);
        }
        else if (tokens[0] == "vt"){
            // * each by a scale to make smaller ( need add )
            // multiply this by the width/ height to scale the 0.0-1.0 range
            TexturePoint tp = TexturePoint(stof(tokens[1])*(ppm.width-1), stof(tokens[2])*(ppm.height-1));
            verTexture.push_back(tp);
        }

        else if (tokens[0] == "f"){
            // get vertices
            int a = stoi(split(tokens[1],'/')[0]) - 1;
            int b = stoi(split(tokens[2],'/')[0]) - 1;
            int c = stoi(split(tokens[3],'/')[0]) - 1;

            ModelTriangle t = ModelTriangle(ver[a], ver[b], ver[c], col);
            // there is a texture value e.g. f 1/1 2/2 3/3
            if (tokens[1].back() != '/'){

                // face1[0] is the vertex, face[1] is the texture point
                string *face1 = split(tokens[1],'/');
                string *face2 = split(tokens[2],'/');
                string *face3 = split(tokens[3],'/');

                
                t.texturePoints[0] = verTexture[stoi(face1[1])-1];
                t.texturePoints[1] = verTexture[stoi(face2[1])-1];
                t.texturePoints[2] = verTexture[stoi(face3[1])-1];
  
            } 
            triangles.push_back(t);             
        }       
    }

    file.close();
    return triangles;
}


vector<Colour> readMTL(string filename){
    ifstream file;
    file.open(filename);

    string line;
    string *tokens;
    string name;

    vector<Colour> colours;

    while (!file.eof()){
        getline(file, line);
        tokens = split(line, ' ');

        if (tokens[0] == "newmtl"){
            name = tokens[1];
        } 
        else if (tokens[0] == "Kd"){
            colours.push_back(Colour(name, int(255 * stof(tokens[1])), int(255 * stof(tokens[2])), int(255 * stof(tokens[3]))));
        }
        else if (tokens[0] == "map_Kd"){
            // tokens[1] should be the texture file name
            colours.push_back(Colour(tokens[1], 0, 0, 0));
        } 
        else {
            // else?
        }
    }
    file.close();
    return colours;
}