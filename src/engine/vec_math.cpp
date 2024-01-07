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



vec2i::vec2i()
{
    this->x = 0.0;
    this->y = 0.0;
}
vec2i::vec2i(int x, int y)
{
    this->x = x;
    this->y = y;
}
vec2i vec2i::operator+(const vec2i& other) const
{
    return {this->x + other.x, this->y + other.y};
}
vec2i vec2i::operator-(const vec2i& other) const
{
    return {this->x - other.x, this->y - other.y};
}
vec2i vec2i::operator*(const vec2i& other) const
{
    return {this->x * other.x, this->y * other.y};
}
vec2i vec2i::operator*(const float& other) const
{
    return {(int)((float)this->x * other), (int)((float)this->y * other)};
}
vec2i vec2i::operator/(const vec2i& other) const
{
    return {this->x / other.x, this->y / other.y};
}
vec2i vec2i::operator/(const float& other) const
{
    return {(int)((float)this->x / other), (int)((float)this->y / other)};
}
vec2i vec2i::operator+=(const vec2i& other)
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}
vec2i vec2i::operator-=(const vec2i& other)
{
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}
vec2i vec2i::operator*=(const vec2i& other)
{
    this->x *= other.x;
    this->y *= other.y;
    return *this;
}
vec2i vec2i::operator*=(const float& other)
{
    this->x *= (int)other;
    this->y *= (int)other;
    return *this;
}
vec2i vec2i::operator/=(const vec2i& other)
{
    this->x /= other.x;
    this->y /= other.y;
    return *this;
}
vec2i vec2i::operator/=(const float& other)
{
    this->x /= (int)other;
    this->y /= (int)other;
    return *this;
}
bool vec2i::operator==(const vec2i& other) const
{
    return this->x == other.x && this->y == other.y;
}
bool vec2i::operator!=(const vec2i& other) const
{
    return this->x != other.x || this->y != other.y;
}
float vec2i::dot(const vec2i& other) const
{
    return (float)this->x * (float)other.x + (float)this->y * (float)other.y;
}
float vec2i::cross(const vec2i& other) const
{
    return (float)this->x * (float)other.y - (float)this->y * (float)other.x;
}
float vec2i::length() const
{
    return sqrt((float)this->x * (float)this->x + (float)this->y * (float)this->y);
}
vec2i vec2i::normalized() const
{
    float l = this->length();
    return {(int)((float)this->x / l), (int)((float)this->y / l)};
}
vec2i vec2i::rotate(float angle) const
{
    return {(int)((float)this->x * cos(angle) - (float)this->y * sin(angle)), (int)((float)this->x * sin(angle) + (float)this->y * cos(angle))};
}
vec2i vec2i::lerp(const vec2i& other, float t) const
{
    return {(int)((float)this->x + ((float)other.x - (float)this->x) * t), (int)((float)this->y + ((float)other.y - (float)this->y) * t)};
}
vec2i vec2i::slerp(const vec2i& other, float t) const
{
    float theta = acos(this->dot(other) / (this->length() * other.length()));
    return (this->rotate(theta * t) * (1.0f - t)) + (other * t);
}
vec2i vec2i::direction_to(const vec2i& other) const
{
    return (other - *this).normalized();
}
float vec2i::distance_to(const vec2i& other) const
{
    return (other - *this).length();
}
float vec2i::angle_to(const vec2i& other) const
{
    return atan2((float)other.y - (float)this->y, (float)other.x - (float)this->x);
}