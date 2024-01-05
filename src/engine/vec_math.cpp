#include "vec_math.h"

vec2::vec2()
{
    this->x = 0.0;
    this->y = 0.0;
}
vec2::vec2(float x, float y)
{
    this->x = x;
    this->y = y;
}
vec2 vec2::operator+(const vec2& other) const
{
    return {this->x + other.x, this->y + other.y};
}
vec2 vec2::operator-(const vec2& other) const
{
    return {this->x - other.x, this->y - other.y};
}
vec2 vec2::operator*(const vec2& other) const
{
    return {this->x * other.x, this->y * other.y};
}
vec2 vec2::operator*(const float& other) const
{
    return {this->x * other, this->y * other};
}
vec2 vec2::operator/(const vec2& other) const
{
    return {this->x / other.x, this->y / other.y};
}
vec2 vec2::operator/(const float& other) const
{
    return {this->x / other, this->y / other};
}
vec2 vec2::operator+=(const vec2& other)
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}
vec2 vec2::operator-=(const vec2& other)
{
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}
vec2 vec2::operator*=(const vec2& other)
{
    this->x *= other.x;
    this->y *= other.y;
    return *this;
}
vec2 vec2::operator*=(const float& other)
{
    this->x *= other;
    this->y *= other;
    return *this;
}
vec2 vec2::operator/=(const vec2& other)
{
    this->x /= other.x;
    this->y /= other.y;
    return *this;
}
vec2 vec2::operator/=(const float& other)
{
    this->x /= other;
    this->y /= other;
    return *this;
}
bool vec2::operator==(const vec2& other) const
{
    return this->x == other.x && this->y == other.y;
}
bool vec2::operator!=(const vec2& other) const
{
    return this->x != other.x || this->y != other.y;
}
float vec2::dot(const vec2& other) const
{
    return this->x * other.x + this->y * other.y;
}
float vec2::cross(const vec2& other) const
{
    return this->x * other.y - this->y * other.x;
}
float vec2::length() const
{
    return sqrt(this->x * this->x + this->y * this->y);
}
vec2 vec2::normalized() const
{
    float l = this->length();
    return {this->x / l, this->y / l};
}
vec2 vec2::rotate(float angle) const
{
    return {this->x * cos(angle) - this->y * sin(angle), this->x * sin(angle) + this->y * cos(angle)};
}
vec2 vec2::lerp(const vec2& other, float t) const
{
    return {this->x + (other.x - this->x) * t, this->y + (other.y - this->y) * t};
}
vec2 vec2::slerp(const vec2& other, float t) const
{
    float theta = acos(this->dot(other) / (this->length() * other.length()));
    return (this->rotate(theta * t) * (1.0f - t)) + (other * t);
}
vec2 vec2::direction_to(const vec2& other) const
{
    return (other - *this).normalized();
}
float vec2::distance_to(const vec2& other) const
{
    return (other - *this).length();
}
float vec2::angle_to(const vec2& other) const
{
    return atan2(other.y - this->y, other.x - this->x);
}