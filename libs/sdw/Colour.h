#pragma once

#include <iostream>

class Colour
{
  public:
    std::string name;
    int red;
    int green;
    int blue;
    int alpha;

    Colour();

    Colour(int r, int g, int b);

    Colour(int r, int g, int b, int a);

    Colour(std::string n, int r, int g, int b);
};

std::ostream& operator<<(std::ostream& os, const Colour& colour);
