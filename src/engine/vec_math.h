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

struct vec2i
{
    int x;
    int y;

    vec2i();
    vec2i(int x, int y);
    vec2i operator+(const vec2i& other) const;
    vec2i operator-(const vec2i& other) const;
    vec2i operator*(const vec2i& other) const;
    vec2i operator*(const float& other) const;
    vec2i operator/(const vec2i& other) const;
    vec2i operator/(const float& other) const;
    vec2i operator+=(const vec2i& other);
    vec2i operator-=(const vec2i& other);
    vec2i operator*=(const vec2i& other);
    vec2i operator*=(const float& other);
    vec2i operator/=(const vec2i& other);
    vec2i operator/=(const float& other);
    bool operator==(const vec2i& other) const;
    bool operator!=(const vec2i& other) const;
    float dot(const vec2i& other) const;
    float cross(const vec2i& other) const;
    float length() const;
    vec2i normalized() const;
    vec2i rotate(float angle) const;
    vec2i lerp(const vec2i& other, float t) const;
    vec2i slerp(const vec2i& other, float t) const;
    vec2i direction_to(const vec2i& other) const;
    float distance_to(const vec2i& other) const;
    float angle_to(const vec2i& other) const;
};


#endif //POMEGRANATE_ENGINE_VEC_MATH_H
