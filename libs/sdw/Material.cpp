#include "Material.h"

Material::Material(){

}

Material::Material(string n, vec3 d, vec3 a, vec3 s, float h, float i){
    name = n;
    diffuse = d;
    ambient = a;
    specular = s;
    highlight = h;
    illum = i;

}