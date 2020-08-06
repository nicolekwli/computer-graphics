#include "Rasterise.h"

vec3 lightPos(0, 0, -5.0);
vec3 lightPower = 16.f * vec3(1, 1, 1);
vec3 indirectLightPowerPerArea = 0.5f * vec3(1, 1, 1);

CanvasPoint vertex3Dto2D(DrawingWindow window, vec3 vertex3D, Camera cam) {

    vec3 point = (vertex3D - cam.cameraPos) * cam.cameraRot;
    
    float x = point.x;
    float y = point.y;
    float z = point.z;

    float x2D = (cam.focalLength * x / (z)) + (window.width/2);
    float y2D = (window.height/2) - (cam.focalLength * y / (z));

    CanvasPoint vertex2D = CanvasPoint(x2D, y2D, 1/z);


    // make sure they are within the screen
    // if (vertex2D.x < 0){
    //     vertex2D.x = 0;
    // } else if (vertex2D.x > window.width-1) {
    //     vertex2D.x = window.width-1;
    // }

    // if (vertex2D.y < 0){
    //     vertex2D.y = 0;
    // } else if (vertex2D.y > window.height-1){
    //     vertex2D.y = window.height-1;
    // }

    return vertex2D;
}

// our little vertex shader?
void modelToCanvasTri(DrawingWindow window, ModelTriangle mt, CanvasTriangle &ct, Camera cam, bool isShade){

    CanvasPoint v0 = vertex3Dto2D(window, mt.vertices[0], cam);
    CanvasPoint v1 = vertex3Dto2D(window, mt.vertices[1], cam);
    CanvasPoint v2 = vertex3Dto2D(window, mt.vertices[2], cam);
    
    if (isShade){
        // calculate new colour for each vertex
        vec3 Ia = vec3(0.25, 0.25, 0.25); // lets say this is the light intensity
        vec3 amb = mt.mat.ambient * Ia;
        // get new colours with lighting info before fillign in triangles
        vec3 diff = mt.mat.diffuse * lightPower * glm::max(dot(mt.normals[0], (lightPos - mt.vertices[0])), 0.0f);
        vec3 illum = amb + diff;
        // vec3 spec = mt.mat.specular * lightPower * ()
        
        v0.c = Colour((int)illum.r, (int)illum.g, (int)illum.b);
        
        diff = mt.mat.diffuse * lightPower * glm::max(dot(mt.normals[1], (lightPos - mt.vertices[1])), 0.0f);
        illum = amb + diff;
        v1.c = Colour((int)illum.r, (int)illum.g, (int)illum.b);
        

        diff = mt.mat.diffuse * lightPower * glm::max(dot(mt.normals[2], (lightPos - mt.vertices[2])), 0.0f);
        illum = amb + diff;
        v2.c = Colour((int)illum.r, (int)illum.g, (int)illum.b);
    } else {
        v0.c = mt.colour;
        v1.c = mt.colour;
        v2.c = mt.colour;
    }
    
    //cout << v2.c << endl;
    ct = CanvasTriangle(v0, v1, v2, mt.colour);

    // each canvas point has a tp
    ct.vertices[0].texturePoint = mt.texturePoints[0];
    ct.vertices[1].texturePoint = mt.texturePoints[1];
    ct.vertices[2].texturePoint = mt.texturePoints[2];
}

vector<CanvasTriangle> clipping(DrawingWindow window, CanvasTriangle ct){
    vector<vec3> norm;
    norm.push_back(vec3(1,0,0)); // this does left
    //norm.push_back(vec3(-1,0,0)); 
    norm.push_back(vec3(0,1,0)); // this does top
    //norm.push_back(vec3(0,-1,0)); 
    norm.push_back(vec3(0,0,1));
    //norm.push_back(vec3(0,0,-1)); 


    vector<CanvasTriangle> final;
    // for each normal
    for (int i=0; i<3; i++){
        float distance;
        vector<vec3> keep;
        vector<vec3> discard;

        // calc each point's distance to norm
        for (int j=0; j<3; j++){
            vec3 v = vec3(ct.vertices[j].x, ct.vertices[j].y, ct.vertices[j].depth);
            distance = dot(v, norm[i]);

            if (distance >= 0){
                keep.push_back(v);
            } else discard.push_back(v);
        }

        if (keep.size() == 3) {
            continue; // break would get out of the for loop
        }

        else if (discard.size() == 3){
            return final; 
        }

        // this doesnt work fully
        else if (keep.size() == 1){
            // get distance ratio to get 2 new points
            float ratio1 = dot(norm[i], discard[0])  / dot(norm[i], (keep[0] - discard[0]));
            float ratio2 = dot(norm[i], discard[1])  / dot(norm[i], (keep[0] - discard[1]));

            vec3 np1 = discard[0] - ratio1 * (keep[0] - discard[0]);
            vec3 np2 = discard[1] - ratio2 * (keep[0] - discard[1]);

            CanvasPoint newP1 = CanvasPoint(np1.x, np1.y, np1.z);
            CanvasPoint newP2 = CanvasPoint(np2.x, np2.y, np2.z);

            CanvasPoint p = CanvasPoint(keep[0].x, keep[0].y, keep[0].z);

            ct.vertices[1] = p;
            ct.vertices[0] = newP1;
            ct.vertices[2] = newP2;

            //final.push_back(CanvasTriangle(p, newP1, newP2, ct.colour));
            //return final;
        }

        else if (keep.size() == 2){
            // get two triangles 
            float ratio1 = dot(norm[i], discard[0])  / dot(norm[i], (keep[0] - discard[0]));
            float ratio2 = dot(norm[i], discard[0])  / dot(norm[i], (keep[1] - discard[0]));

            vec3 np1 = discard[0] - ratio1 * (keep[0] - discard[0]);
            vec3 np2 = discard[0] - ratio2 * (keep[1] - discard[0]);

            CanvasPoint newP1 = CanvasPoint(np1.x, np1.y, np1.z);
            CanvasPoint newP2 = CanvasPoint(np2.x, np2.y, np2.z);

            CanvasPoint p1 = CanvasPoint(keep[0].x, keep[0].y, keep[0].z);
            CanvasPoint p2 = CanvasPoint(keep[1].x, keep[1].y, keep[1].z);


            vector<CanvasTriangle> clipped1 = clipping(window, CanvasTriangle(p1, p2, newP1, ct.colour));
            vector<CanvasTriangle> clipped2 = clipping(window, CanvasTriangle(p2, newP1, newP2, ct.colour));

            for (int a=0; a<clipped1.size(); a++) final.push_back(clipped1[a]);
            for (int b=0; b<clipped2.size(); b++) final.push_back(clipped2[b]);
            return final;
        }
    }
    final.push_back(ct);
    return final;
}

vector<CanvasTriangle> clippingFrus(DrawingWindow window, Camera c, CanvasTriangle ct){
    vector<CanvasTriangle> final;
    // for each side 
    for (int i = 0; i < 6; i++){
        float distance;
        vector<vec3> keep;
        vector<vec3> discard;
        for (int j=0; j<3; j++){
            vec3 v = vec3(ct.vertices[j].x, ct.vertices[j].y, ct.vertices[j].depth);
            //distance = dotproduct (point, plane.normal) - plane.distance;
            // (currentp - p) * n
            distance = dot(c.f.sides[i].normal, (v-c.f.sides[i].point));
            // or (currentp * n) - d
                // d = p*n
            //distance = dot(c.f.sides[i].normal, v);
            //distance = dot(v, c.f.sides[i].normal);

            if (distance >= 0){
                keep.push_back(v);
            } else discard.push_back(v);
        }

        if (keep.size() == 3) {
            continue; // break would get out of the for loop i think
        }


        else if (discard.size() == 3){
            // final.push_back(ct); // this hsouldnt be here but it make up and left work
            return final; 
            //break;
        }

        // this doesnt work fully
        else if ((discard.size() == 2) && (keep.size() == 1)){
            // get distance ratio to get 2 new points
            float ratio1 = dot(c.f.sides[i].normal, discard[0])  / dot(c.f.sides[i].normal, keep[0] - discard[0]);
            float ratio2 = dot(c.f.sides[i].normal, discard[1])  / dot(c.f.sides[i].normal, keep[0] - discard[1]);

            vec3 np1 = discard[0] - ratio1 * (keep[0] - discard[0]);
            vec3 np2 = discard[1] - ratio2 * (keep[0] - discard[1]);

            CanvasPoint newP1 = CanvasPoint(np1.x, np1.y, np1.z);
            CanvasPoint newP2 = CanvasPoint(np2.x, np2.y, np2.z);

            CanvasPoint p = CanvasPoint(keep[0].x, keep[0].y, keep[0].z);

            ct.vertices[0] = p;
            ct.vertices[1] = newP1;
            ct.vertices[2] = newP2;

            // final.push_back(CanvasTriangle(p, newP1, newP2, ct.colour));
            // return final;
        }

        else if ((keep.size() == 2) && (discard.size() == 1)){
            // get two triangles 
            float ratio1 = dot(c.f.sides[i].normal, discard[0])  / dot(c.f.sides[i].normal, (keep[0] - discard[0]));
            float ratio2 = dot(c.f.sides[i].normal, discard[0])  / dot(c.f.sides[i].normal, (keep[1] - discard[0]));

            vec3 np1 = discard[0] - ratio1 * (keep[0] - discard[0]);
            vec3 np2 = discard[0] - ratio2 * (keep[1] - discard[0]);

            CanvasPoint newP1 = CanvasPoint(np1.x, np1.y, np1.z);
            CanvasPoint newP2 = CanvasPoint(np2.x, np2.y, np2.z);

            CanvasPoint p1 = CanvasPoint(keep[0].x, keep[0].y, keep[0].z);
            CanvasPoint p2 = CanvasPoint(keep[1].x, keep[1].y, keep[1].z);

            //final.push_back(CanvasTriangle(p1, p2, newP1, ct.colour));
            //final.push_back(CanvasTriangle(p2, newP1, newP2, ct.colour));

            vector<CanvasTriangle> clipped1 = clipping(window, CanvasTriangle(p1, p2, newP1, ct.colour));
            vector<CanvasTriangle> clipped2 = clipping(window, CanvasTriangle(p2, newP1, newP2, ct.colour));

            for (int a=0; a<clipped1.size(); a++) final.push_back(clipped1[a]);
            for (int b=0; b<clipped2.size(); b++) final.push_back(clipped2[b]);
            return final;
        }
        
    }
    final.push_back(ct);
    return final;
}


void createWireframe(DrawingWindow window, vector<ModelTriangle> t, Camera cam){
    vector<CanvasTriangle> canvasTriangles; 
    // for each triangle we need to get the canvas triangle first
    // then draw each triangle
    for (std::vector<int>::size_type i = 0; i != t.size(); i++){
        CanvasTriangle ct; 
 
        modelToCanvasTri(window, t[i], ct, cam, false);
        canvasTriangles.push_back(ct);

        vector<CanvasTriangle> cts = clipping(window, ct);
        for (int j = 0; j < cts.size(); j++){
            drawStrokedTriangle(window, cts[j]);
        }
    }
}

// each triangle has a colour name ppm
void rasterise(DrawingWindow window, vector<ModelTriangle> t, Camera cam, vector<vector<uint32_t>> pixels, vector<Material> material, int kind){
    vector<CanvasTriangle> canvasTriangles; 
    for (std::vector<int>::size_type i = 0; i != t.size(); i++){
        CanvasTriangle ct; 

        modelToCanvasTri(window, t[i], ct, cam, false);
        canvasTriangles.push_back(ct);

        //vector<CanvasTriangle> cts = clipping(window, ct);
        vector<CanvasTriangle> cts = clippingFrus(window, cam, ct);


        for (int j = 0; j < cts.size(); j++){
            // sort vertices
            for (int i = 0; i < 3; i++){
                if (cts[j].vertices[2].y < cts[j].vertices[0].y){
                    swap(cts[j].vertices[2], cts[j].vertices[0]);
                }
                if (cts[j].vertices[1].y < cts[j].vertices[0].y){
                    swap(cts[j].vertices[0], cts[j].vertices[1]);
                }
                if (cts[j].vertices[2].y < cts[j].vertices[1].y){
                    swap(cts[j].vertices[1], cts[j].vertices[2]);
                }
            }
            // calculate normal of triangle mesh and poitns

            // need to make them all vec3
            //cts[j].normal = cross((cts[j].vertices[2] - cts[j].vertices[0]), (cts[j].vertices[1] - cts[j].vertices[0])); // surfac normal
            // vec3 v = vec3(cts.vertices[j].x, cts.vertices[j].y, cts.vertices[j].depth);
            // cts[j].vertices[0].normal = normalize(cam.cameraPos - v);
            // cts[j].vertices[0].normal = ;
            // cts[j].vertices[0].normal = ;

            //drawFilledTriangle(window, ct.colour, cts[j]);
            if (kind == 1){ // fill triangles
                //cout << ct.colour << endl;
                drawFilledTriangle(window, ct.colour, cts[j], false);
            } else if (kind == 2){ // texture
                fillTextureTriangle(window, pixels, cts[j]);
            } else if (kind == 3) { // gouraud

                
                drawFilledTriangle(window, ct.colour, cts[j], true);

            }
            // if (ct.vertices[0].texturePoint.x == -1){
            //     drawFilledTriangle(window, ct.colour, cts[j]);
            // } else {
            //     fillTextureTriangle(window, pixels, cts[j]);
            // }  
        }
    }

        
}