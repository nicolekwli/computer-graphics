#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace glm;

using glm::vec3;


class Material {
    public:
        string name;
        vec3 diffuse; //kd -diffuse
        vec3 ambient; //ka
        vec3 specular; //ks
        float highlight; //ns
        float illum;


        Material(); //constructor
        Material(string n, vec3 d, vec3 a, vec3 s, float h, float i);

};