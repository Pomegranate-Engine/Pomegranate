#ifndef POMEGRANATE_ENGINE_VEC_MATH_H
#define POMEGRANATE_ENGINE_VEC_MATH_H
#include<cstdio>
#include <iostream>

struct vec2
{
    float x;
    float y;

    vec2();
    vec2(float x, float y);
    vec2 operator+(const vec2& other) const;
    vec2 operator-(const vec2& other) const;
    vec2 operator*(const vec2& other) const;
    vec2 operator*(const float& other) const;
    vec2 operator/(const vec2& other) const;
    vec2 operator/(const float& other) const;
    vec2 operator+=(const vec2& other);
    vec2 operator-=(const vec2& other);
    vec2 operator*=(const vec2& other);
    vec2 operator*=(const float& other);
    vec2 operator/=(const vec2& other);
    vec2 operator/=(const float& other);
    bool operator==(const vec2& other) const;
    bool operator!=(const vec2& other) const;
    float dot(const vec2& other) const;
    float cross(const vec2& other) const;
    float length() const;
    vec2 normalized() const;
    vec2 rotate(float angle) const;
    vec2 lerp(const vec2& other, float t) const;
    vec2 slerp(const vec2& other, float t) const;
    vec2 direction_to(const vec2& other) const;
    float distance_to(const vec2& other) const;
    float angle_to(const vec2& other) const;
};


#endif //POMEGRANATE_ENGINE_VEC_MATH_H
