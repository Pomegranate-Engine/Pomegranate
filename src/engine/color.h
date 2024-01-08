#ifndef POMEGRANATE_ENGINE_COLOR_H
#define POMEGRANATE_ENGINE_COLOR_H

class Color
{
public:
    int r,g,b,a;
    Color();
    Color(int r, int g, int b, int a=255);
    explicit Color(int hex);
    Color(float r, float g, float b, float a=1);
    Color operator+(const Color& other);
    Color operator-(const Color& other);
    Color operator*(float other);
    Color operator/(float other);
};

#endif //POMEGRANATE_ENGINE_COLOR_H
