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

Color::Color(int hex)
{
    this->r = (int)(hex & 0xFF000000) >> 24;
    this->g = (int)(hex & 0x00FF0000) >> 16;
    this->b = (int)(hex & 0x0000FF00) >> 8;
    this->a = (int)(hex & 0x000000FF);
}

Color::Color(float r, float g, float b, float a)
{
    this->r = (int)(r*255);
    this->g = (int)(g*255);
    this->b = (int)(b*255);
    this->a = (int)(a*255);
}

Color Color::operator+(const Color &other) const
{
    return {this->r+other.r, this->g+other.g, this->b+other.b, this->a+other.a};
}

Color Color::operator-(const Color &other) const
{
    return {this->r-other.r, this->g-other.g, this->b-other.b, this->a-other.a};
}

Color Color::operator*(float other) const
{
    return {((float)this->r/255.0f)*other, ((float)this->g/255.0f)*other, ((float)this->b/255.0f)*other, ((float)this->a/255.0f)*other};
}

Color Color::operator/(float other) const
{
    return {((float)this->r/255.0f)/other, ((float)this->g/255.0f)/other, ((float)this->b/255.0f)/other, ((float)this->a/255.0f)/other};
}

Color Color::mix(const Color &other, float amount) const
{
    return *this * (1.0f - amount) + other * amount;
}

Color::Color(float hue, float brightness, float a)
{
    float c = brightness;
    float x = c * (1 - abs(fmod(hue / 60.0, 2) - 1));
    float m = brightness - c;
    if (hue >= 0 && hue < 60)
    {
        this->r = (c + m) * 255;
        this->g = (x + m) * 255;
        this->b = m * 255;
    }
    else if (hue >= 60 && hue < 120)
    {
        this->r = (x + m) * 255;
        this->g = (c + m) * 255;
        this->b = m * 255;
    }
    else if (hue >= 120 && hue < 180)
    {
        this->r = m * 255;
        this->g = (c + m) * 255;
        this->b = (x + m) * 255;
    }
    else if (hue >= 180 && hue < 240)
    {
        this->r = m * 255;
        this->g = (x + m) * 255;
        this->b = (c + m) * 255;
    }
    else if (hue >= 240 && hue < 300)
    {
        this->r = (x + m) * 255;
        this->g = m * 255;
        this->b = (c + m) * 255;
    }
    else if (hue >= 300 && hue < 360)
    {
        this->r = (c + m) * 255;
        this->g = m * 255;
        this->b = (x + m) * 255;
    }
    this->a = a * 255;
}
