#include "Ppm.h"

PPM::PPM(){

}

PPM::PPM(int w, int h, vector<vector<uint32_t>> p){
    width = w;
    height = h;
    pixels = p;

}