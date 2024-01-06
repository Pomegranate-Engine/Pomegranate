#include "color.h"

Color::Color()
{
    this->r = 255;
    this->g = 255;
    this->b = 255;
    this->a = 255;
}

Color::Color(int r, int g, int b, int a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Color::Color(int r, int g, int b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 255;
}

Color::Color(int hex)
{
    this->r = (hex & 0xFF000000) >> 24;
    this->g = (hex & 0x00FF0000) >> 16;
    this->b = (hex & 0x0000FF00) >> 8;
    this->a = (hex & 0x000000FF);
}

Color::Color(float r, float g, float b, float a)
{
    this->r = (int)(r*255);
    this->g = (int)(g*255);
    this->b = (int)(b*255);
    this->a = (int)(a*255);
}

Color::Color(float r, float g, float b)
{
    this->r = (int)(r*255);
    this->g = (int)(g*255);
    this->b = (int)(b*255);
    this->a = 255;
}

Color Color::operator+(const Color &other)
{
    return {this->r+other.r, this->g+other.g, this->b+other.b, this->a+other.a};
}

Color Color::operator-(const Color &other)
{
    return {this->r-other.r, this->g-other.g, this->b-other.b, this->a-other.a};
}

Color Color::operator*(float other)
{
    return {((float)this->r/255.0f)*other, ((float)this->g/255.0f)*other, ((float)this->b/255.0f)*other, ((float)this->a/255.0f)*other};
}

Color Color::operator/(float other)
{
    return {((float)this->r/255.0f)/other, ((float)this->g/255.0f)/other, ((float)this->b/255.0f)/other, ((float)this->a/255.0f)/other};
}
