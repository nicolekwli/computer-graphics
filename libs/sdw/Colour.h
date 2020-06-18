#pragma once

#include <iostream>

class Colour
{
  public:
    std::string name;
    int red;
    int green;
    int blue;

    Colour();

    Colour(int r, int g, int b);

    Colour(std::string n, int r, int g, int b);
};

std::ostream& operator<<(std::ostream& os, const Colour& colour);
