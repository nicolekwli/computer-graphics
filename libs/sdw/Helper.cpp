#include "Helper.h"

vec3 camP;
int shadeKind;
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

    uint32_t a_colour = bitpackingColour(a.c);
    uint32_t b_colour = bitpackingColour(b.c);
    vec3 ac = vec3(a.c.red, a.c.green, a.c.blue);
    vec3 bc = vec3(b.c.red, b.c.green, b.c.blue);
    vec3 intervalsC = (bc - ac) / (noOfVals - 1.0f);

    for (int i = 1; i < noOfVals; i++) {
        p = CanvasPoint(a.x + intervalsX * i, a.y + intervalsY * i) ;
        float q = (p.x - a.x) / (b.x - a.x);
        p.depth = 1 / a.depth * (1-q) + 1 / b.depth * q;
        p.depth = 1/ p.depth;

        texPointCorrected(a, b, p);
        
        // INTERPOLATE COLOUR AS WELL
        if (shadeKind == 3 ) {
            if (a_colour == b_colour){
                p.c = a.c;
            } else {
                p.c = Colour(int(ac.r * (1-q) + bc.r * q), int (ac.g * (1-q) + bc.g * q), int(ac.b * (1-q) + bc.b * q));
            }
        } 
        else if (shadeKind == 4){
            // INTERPOLATE NORMAL
            p.normal = normalize(vec3(a.normal.x * (1-q) + b.normal.x * q, a.normal.y * (1-q) + b.normal.y * q, a.normal.z * (1-q) + b.normal.z * q));
            p.mat = b.mat;
        }
        vect.push_back(p);
    }
    vect.push_back(b);
    return vect;
}

void texPointCorrected(CanvasPoint a, CanvasPoint b, CanvasPoint &newP){
    float shortD = sqrt(pow((newP.x - a.x), 2) + pow((newP.y - a.y), 2));
    float longD = sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2));
    float q = shortD / longD;
    float z = 1 / ((1 / a.depth * (1-q))  + (1 / b.depth * q));
    float texX = abs(((a.texturePoint.x * (1 - q) / a.depth) + (b.texturePoint.x * q / b.depth))*newP.depth);
    float texY = abs(((a.texturePoint.y * (1 - q) / a.depth) + (b.texturePoint.y * q / b.depth)) *newP.depth);
    newP.texturePoint.x = texX;
    newP.texturePoint.y = texY;
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


// Modified https://inst.eecs.berkeley.edu/~cs150/fa10/Lab/CP3/LineDrawing.pdf adding depth
void drawLineB(DrawingWindow window, CanvasPoint p1, CanvasPoint p2, Colour c){
    uint32_t colour = bitpackingColour(c);
    
    int dx = p2.x - p1.x;
    if (dx == 0){
        drawLine(window, p1, p2, c);
    } else {
        bool steep = abs(p2.y - p1.y) > abs(p2.x - p1.x);
        if (steep){
            swap(p1.x, p1.y);
            swap(p2.x, p2.y);
        }

        if (p2.x < p1.x){
            swap(p1,p2);
        }

        int dErr = abs(p2.y - p1.y);
        int yStep = p1.y > p2.y ? -1 : 1;
        
        int dx = p2.x - p1.x;

        float dStep = (p2.depth - p1.depth) / dx;
        float d = p1.depth;

        int err = dx / 2;
        int y = p1.y;

        for (int i=p1.x; i<p2.x; i++){
            if (steep){
                window.setPixelColour(y, i, d, colour);
            } else {
                window.setPixelColour(i, y, d, colour);
            }
            
            err -= dErr;
            if (err < 0){
                y += yStep;
                err += dx;
            }
            d += dStep;
            
        }
    }
}

void drawLineWu(DrawingWindow window, CanvasPoint p1, CanvasPoint p2, Colour c){
    uint32_t colour = bitpackingColour(c);
    
    int dx = p2.x - p1.x;
    if (dx == 0){
        drawLine(window, p1, p2, c);
    } else {
        // wu line
        bool steep = abs(p2.y - p1.y) > abs(p2.x - p1.x);

        if (steep){
            swap(p1.x, p1.y);
            swap(p2.x, p2.y);
        }

        if (p2.x < p1.x){
            swap(p1,p2);
        }

        int dx = p2.x - p1.x;
        int dy = p2.y - p1.y;
        float gradient = (float)dy/dx;

        int xend = round(p1.x);
        float yend = (float) (p1.y + gradient * (xend - p1.x));
        float xgap = (float) (1 - (abs(p1.x + 0.5) - floor(abs(p1.x + 0.5))));

        int xpxl1 = xend;
        int ypxl1 = floor(yend);

        float alpha;
        uint32_t newColour;

        if (steep) {
            alpha = (1 - (abs(yend) - floor(abs(yend)))) * xgap;
            newColour = (((uint32_t) floor(255*alpha)) << 24)| colour;
            window.setPixelColour(ypxl1, xpxl1, p1.depth, newColour);

            alpha = floor(yend) * xgap;
            newColour = (((uint32_t) floor(255*alpha)) << 24)| colour;
            window.setPixelColour(ypxl1+1, xpxl1, p1.depth, newColour);
        } else {
            alpha = (1 - (abs(yend) - floor(abs(yend)))) * xgap;
            
            newColour = (((uint32_t) floor(255*alpha)) << 24)| colour;
            window.setPixelColour(xpxl1, ypxl1, p1.depth, newColour);

            alpha = floor(yend) * xgap;
            newColour = (((uint32_t) floor(255*alpha)) << 24)| colour;
            window.setPixelColour(xpxl1, ypxl1+1, p1.depth, newColour);
        }

        double intery = yend + gradient;
        xend = round(p2.x);
        yend = p2.y + gradient * (xend - p2.x);
        xgap = 1 - (abs(p2.x + 0.5) - floor(abs(p2.x + 0.5)));
        int xpxl2 = xend;
        int ypxl2 = floor(yend);

        alpha = (1 - (abs(yend) - floor(abs(yend)))) * xgap;
        newColour = (((uint32_t) floor(255*alpha))<< 24)| colour;

        if (steep) {
            window.setPixelColour(ypxl2, xpxl2, p2.depth, newColour);
            window.setPixelColour(ypxl2 + 1, xpxl2, p2.depth, newColour);
        } else {
            window.setPixelColour(xpxl2, ypxl2, p2.depth, newColour);
            window.setPixelColour(xpxl2, ypxl2+1, p2.depth, newColour);
        }

        float dStep = (p2.depth - p1.depth) / dx;
        float d = p1.depth;
        // draw line 
        for (int x = xpxl1 + 1; x <= xpxl2; x ++) {
            if (steep) {
                alpha = (1 - (abs(intery) - floor(abs(intery))));
                newColour = (((uint32_t) floor(255*alpha)) << 24)| colour;
                window.setPixelColour(floor(intery), x, d, newColour);

                alpha = abs(intery) - floor(abs(intery));
                newColour = (((uint32_t) floor(255*alpha))<< 24)| colour;
                window.setPixelColour(floor(intery) + 1, x, d, newColour);
            } else {
                alpha = (1 - (abs(intery) - floor(abs(intery))));
                newColour = (((uint32_t) floor(255*alpha)) << 24)| colour;
                window.setPixelColour(x, floor(intery), d, newColour);

                alpha = (abs(intery) - floor(abs(intery)));
                newColour = (((uint32_t) floor(255*alpha)) << 24)| (colour & 0x00ffffff);
                window.setPixelColour(x, floor(intery)+1, d, newColour);
            }
            d += dStep;
            intery = intery + gradient;
        }
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

    drawLineWu(window, t.vertices[1], t.vertices[0], t.colour);
    drawLineWu(window, t.vertices[2], t.vertices[0], t.colour);
    drawLineWu(window, t.vertices[1], t.vertices[2], t.colour);
}


void fillTriangle(DrawingWindow window, vector<CanvasPoint> lineTopLeft, vector<CanvasPoint> lineTopRight, Colour c, int kind) {
    uint32_t colour = bitpackingColour(c);

    for (float a = 0.0; a<lineTopLeft.size(); a++){
        //drawLine(window, lineTopLeft[a], lineTopRight[a], c);
        //drawLineB(window, lineTopLeft[a], lineTopRight[a], c);
        //drawLineWu(window, lineTopLeft[a], lineTopRight[a], c);

        if (kind == 3){ // gouraud
            vector<CanvasPoint> line = interpolation(lineTopLeft[a], lineTopRight[a], abs(lineTopLeft[a].x - lineTopRight[a].x));
            for (int i=0; i<line.size(); i++){
                colour = bitpackingColour(line[i].c);
                window.setPixelColour(line[i].x, line[i].y, line[i].depth, colour);
            }
        } 
        else if (kind == 4){ // phong
            // itnerpolate normal
            vector<CanvasPoint> line = interpolation(lineTopLeft[a], lineTopRight[a], abs(lineTopLeft[a].x - lineTopRight[a].x));
            // calculate colour
            for (int i=0; i<line.size(); i++){
                // do illumination model 
                vec3 lightPos(-1, 5, -3.0);
                vec3 lightPower = 165.5f * vec3(1, 1, 1);
                vec3 Ia = vec3(0.55, 0.55, 0.55);
                vec3 amb = line[i].mat.ambient * Ia;

                float distance = pow((lightPos.x - line[i].x),2) + pow((lightPos.y - line[i].y),2) + pow((lightPos.z - line[i].depth),2);
                distance = sqrt(distance);

                vec3 pos = vec3(line[i].x, line[i].y, line[i].depth);
                vec3 diff = line[i].mat.diffuse * glm::max(dot(line[i].normal, (lightPos - pos)), 0.0f) * (lightPower / (4.0f * 3.14f*distance*distance));

                vec3 tolight = normalize(lightPos - pos);
                vec3 R = normalize(2.0f * line[i].normal * dot(tolight, line[i].normal) - tolight); // this is right
                vec3 V = normalize(camP- line[i].normal); // ??
                vec3 spec = line[i].mat.specular * powf(dot(R, V), line[i].mat.highlight) * vec3(1.0f);
                vec3 illum = amb + diff + spec;
                // setPixelColour
                colour = bitpackingColour(Colour((int)illum.r, (int)illum.g, (int)illum.b));
                window.setPixelColour(line[i].x, line[i].y, line[i].depth, colour);
            }
        }
            
        else {
            drawLineWu(window, lineTopLeft[a], lineTopRight[a], c);
        }
    }
}

void drawFilledTriangle(DrawingWindow window, Colour c, CanvasTriangle triangle, int kind, vec3 cameraPos){
    camP = cameraPos;
    shadeKind = kind;
    
    float ratio = (triangle.vertices[1].y - triangle.vertices[0].y) / (triangle.vertices[2].y - triangle.vertices[0].y);
    CanvasPoint newP = CanvasPoint(triangle.vertices[0].x + ratio * (triangle.vertices[2].x - triangle.vertices[0].x), 
                                        triangle.vertices[0].y + ratio * (triangle.vertices[2].y - triangle.vertices[0].y),
                                        triangle.vertices[0].depth + ratio * (triangle.vertices[2].depth - triangle.vertices[0].depth));
    uint32_t colour = bitpackingColour(newP.c);
    uint32_t colour1 = bitpackingColour(triangle.vertices[0].c);
    uint32_t colour2 = bitpackingColour(triangle.vertices[2].c);

    colour = colour1 + ratio * (colour2 - colour1);
    newP.c = triangle.vertices[0].c;

    newP.normal = triangle.vertices[0].normal + ratio * (triangle.vertices[2].normal - triangle.vertices[0].normal);
    newP.mat = triangle.vertices[0].mat;

    // make sure newP has a smaller value x than vertice 1
    if (newP.x > triangle.vertices[1].x){
        swap(newP, triangle.vertices[1]);
    }

    //drawStrokedTriangle(window, triangle);

    // Fill top triangle
    vector<CanvasPoint> lineTopLeft = interpolation(triangle.vertices[0], newP, abs(triangle.vertices[0].y - triangle.vertices[1].y)+1);
    vector<CanvasPoint> lineTopRight = interpolation(triangle.vertices[0], triangle.vertices[1], abs(triangle.vertices[0].y - triangle.vertices[1].y)+1);
    fillTriangle(window, lineTopLeft, lineTopRight, triangle.colour, kind);

    // Bottom triangle
    vector<CanvasPoint> lineBottomLeft = interpolation(newP,triangle.vertices[2], abs(triangle.vertices[2].y - triangle.vertices[1].y)+1);
    vector<CanvasPoint> lineBottomRight = interpolation(triangle.vertices[1], triangle.vertices[2], abs(triangle.vertices[2].y - triangle.vertices[1].y)+1);
    fillTriangle(window,lineBottomLeft, lineBottomRight, triangle.colour, kind); 
}


// to/ from
void fillTexture(DrawingWindow window, vector<CanvasPoint> lineTopLeft, vector<CanvasPoint> lineTopRight, vector<vector<uint32_t>> pixels) {
    int steps;
    // make sure we fill according to the longer line so it fills
    for (float a = 0.0; a<lineTopLeft.size(); a++){
        steps = (int) abs(lineTopLeft[a].x - lineTopRight[a].x);
        // cout << "steps" << endl;
        // cout << steps << endl;
        vector<CanvasPoint> points = interpolation(lineTopLeft[a], lineTopRight[a], steps+1);
        // drawLine(lineTopLeft[a], lineTopRight, pixels[points[c].texturePoint.y][points[c].texturePoint.x]);
        for (int c = 0; c < steps; c++){
            // cout << c << endl;
            // cout << "size" << endl;
            // cout << points.size() << endl;
            // cout << points[c].texturePoint.y << endl;
            window.setPixelColour((int)points[c].x, (int)points[c].y, points[c].depth, pixels[points[c].texturePoint.y][points[c].texturePoint.x]);
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

    // CanvasPoint newP = CanvasPoint(t.vertices[0].x + ratio * (t.vertices[2].x - t.vertices[0].x), 
    //                                     t.vertices[0].y + ratio * (t.vertices[2].y - t.vertices[0].y));

    // depth
    float q = (newP.x - t.vertices[0].x) / (t.vertices[2].x - t.vertices[0].x);
    newP.depth = 1 / t.vertices[0].depth * (1-q) + 1 / t.vertices[2].depth * q;
    newP.depth = 1/ newP.depth;

    // get texture points
    texPointCorrected(t.vertices[0], t.vertices[2], newP);
    
    // float scale = (t.vertices[0].y-t.vertices[1].y) / (t.vertices[0].y-t.vertices[2].y);
    // newP.texturePoint.x = t.vertices[0].texturePoint.x - scale * (t.vertices[0].texturePoint.x - t.vertices[2].texturePoint.x);
    // newP.texturePoint.y = t.vertices[0].texturePoint.y - scale * (t.vertices[0].texturePoint.y - t.vertices[2].texturePoint.y);

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

    file.close();

    PPM ppm = PPM(width, height, pixels);

    return ppm;
}

void savePPM(DrawingWindow window, string filename){
    ofstream file;
    file.open(filename, ios::out | ios::binary);
    if (!file) {
        cerr << "Cannot open file" << endl;
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

        if ((tokens[0] == "o")){ // || (tokens[1] == "Object")){
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

        // sorry not the best way to do this
        else if (tokens[0] == "f"){
            // get vertices
            int a = stoi(split(tokens[1],'/')[0]) - 1;
            int b = stoi(split(tokens[2],'/')[0]) - 1;
            int c = stoi(split(tokens[3],'/')[0]) - 1;

            ModelTriangle t = ModelTriangle(ver[a], ver[b], ver[c], col);
            t.objname = objectName;
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

// This function reads obj files by reading all vertices and normals then creates the ModelTriangles
// Used for cornell-boxes-extra
vector<ModelTriangle> readOBJAlt(string filename, vector<Material> mtls, PPM ppm, float rescale){

    string line;
    string *tokens;
    string objectName;
    vector<vec3> Vs;
    vector<vec3> VNs;
    vector<TexturePoint> VTs;
    Material material;
    vector<ModelTriangle> triangles;

    ifstream file;
    file.open(filename);

    while (getline(file, line)){
        tokens = split(line, ' ');

        if ((tokens[0] == "#") || (tokens[0] == "g")){
            continue;
        }
        else if ((tokens[0] == "o")){
            objectName = tokens[1];
        }
        else if (tokens[0] == "usemtl") {
            // store material
            for (int i = 0; i < mtls.size(); i++){
                if(mtls[i].name == tokens[1]){
                    material = mtls[i];
                }
            }
        }  else if (tokens[0] == "v"){
            glm::vec3 vec = vec3(stof(tokens[2])*rescale, stof(tokens[3])*rescale, stof(tokens[4])*-rescale);
            Vs.push_back(vec);
        } else if (tokens[0] == "vn"){
            glm::vec3 vec = vec3(stof(tokens[1])*rescale, stof(tokens[2])*rescale, stof(tokens[3])*-rescale);
            VNs.push_back(vec);
        } else if (tokens[0] == "vt"){
            TexturePoint tp = TexturePoint(stof(tokens[1])*(ppm.width-1), stof(tokens[2])*(ppm.height-1));
            VTs.push_back(tp);
        }
        else if (tokens[0] == "f"){
            ModelTriangle t;
            std::size_t found = tokens[1].find("//");
            // v and vn
            if (found != std::string::npos){
                int a = stoi(split(tokens[1],'//')[0]) - 1;
                int b = stoi(split(tokens[2],'//')[0]) - 1;
                int c = stoi(split(tokens[3],'//')[0]) - 1;

                t = ModelTriangle(Vs[a], Vs[b], Vs[c], Colour(material.diffuse.r, material.diffuse.g, material.diffuse.b));
                t.objname = objectName;

                t.mat.name = material.name;
                t.mat.ambient = material.ambient;
                t.mat.diffuse = material.diffuse;
                t.mat.highlight = material.highlight;
                t.mat.illum = material.illum;
                t.mat.specular = material.specular;

                string *face1 = split(tokens[1],'/');
                string *face2 = split(tokens[2],'/');
                string *face3 = split(tokens[3],'/');

                t.normals[0] = VNs[stoi(face1[2])-1];
                t.normals[1] = VNs[stoi(face2[2])-1];
                t.normals[2] = VNs[stoi(face3[2])-1];
            } else {
                // get vertices
                int a = stoi(split(tokens[1],'/')[0]) - 1;
                int b = stoi(split(tokens[2],'/')[0]) - 1;
                int c = stoi(split(tokens[3],'/')[0]) - 1;

                t = ModelTriangle(Vs[a], Vs[b], Vs[c], Colour(material.diffuse.r, material.diffuse.g, material.diffuse.b));
                // t.mat = material // should do the same
                t.objname = objectName;

                t.mat.name = material.name;
                t.mat.ambient = material.ambient;
                t.mat.diffuse = material.diffuse;
                t.mat.highlight = material.highlight;
                t.mat.illum = material.illum;
                t.mat.specular = material.specular;

                // there is a texture value e.g. f 1/1 2/2 3/3
                // or 1/1/1
                if (tokens[1].back() != '/'){
                    // face1[0] is the vertex, face[1] is the texture point
                    string *face1 = split(tokens[1],'/');
                    string *face2 = split(tokens[2],'/');
                    string *face3 = split(tokens[3],'/');
                    
                    t.normals[0] = VNs[stoi(face1[1])-1];
                    t.normals[1] = VNs[stoi(face2[1])-1];
                    t.normals[2] = VNs[stoi(face3[1])-1];

                    t.texturePoints[0] = VTs[stoi(face1[2])-1];
                    t.texturePoints[1] = VTs[stoi(face2[2])-1];
                    t.texturePoints[2] = VTs[stoi(face3[2])-1];
                } 
            }
            triangles.push_back(t);         
        }   
    }
    return triangles;
}


vector<Colour> readMTL(string filename){
    ifstream file;
    file.open(filename);

    string line;
    string *tokens;
    string name;

    vector<Colour> colours;

    while (getline(file, line)){
        //getline(file, line);
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
        } else continue;
    }
    file.close();
    return colours;
}

// Used for cornell-box-extras
vector<Material> readMTLAlt(string filename){
    ifstream file;
    file.open(filename);

    string line;
    string *tokens;
    string name;

    vector<Material> final;
    
    vector<Colour> colours;
    Material mat;

    while (getline(file, line)){
        tokens = split(line, ' ');

        if (tokens[0] == "newmtl"){
            mat.name = tokens[1];
        } 
        else if (tokens[0] == "Ka"){
            mat.ambient = vec3(int(255 * stof(tokens[1])), int(255 * stof(tokens[2])), int(255 * stof(tokens[3])));
        }
        else if (tokens[0] == "Kd"){
            mat.diffuse = vec3(int(255 * stof(tokens[1])), int(255 * stof(tokens[2])), int(255 * stof(tokens[3])));
        }
        else if (tokens[0] == "Ks"){
            mat.specular = vec3(int(255 * stof(tokens[1])), int(255 * stof(tokens[2])), int(255 * stof(tokens[3])));
        }
        else if (tokens[0] == "Ns"){
            mat.highlight = stof(tokens[1]);
        }
        else if (tokens[0] == "illum"){
            mat.illum = stof(tokens[1]);
        }
        else if (tokens[0] == "map_Kd"){
            // tokens[1] should be the texture file name
            colours.push_back(Colour(tokens[1], 0, 0, 0));
        }     
        final.push_back(mat);
    }
    file.close();
    return final;
}

// once rasterising is done at a higher resolution this should work fingers crossed
void SSAA(DrawingWindow window){
    uint32_t p, sp; // pixel, subpixel
    uint32_t r = 0;
    uint32_t g = 0;
    uint32_t b = 0;
    int samples= 3*3; // the number making up one pixel

    for (int y = 1; y < window.height-1; y++) {
        for (int x = 1; x < window.width-1; x++) {
            r,g,b=0;
            // get colour from the area that would contribute to final pixel
            for (int suby = -1; suby < 2; suby++) {
                for (int subx = -1; subx < 2; subx++) {
                    sp = window.getPixelColour(x + subx, y + suby);
                    r += (sp & 0x00ff0000);
                    g += (sp & 0x0000ff00);
                    b += (sp & 0x000000ff);
                }
            }
            // avg it out 
            p = 0xff000000 | ((r/samples)& 0x00ff0000) | ((g/samples)& 0x0000ff00) | ((b/samples)& 0x000000ff);
            window.setPixelColour(x,y,p);
        }
    }

}