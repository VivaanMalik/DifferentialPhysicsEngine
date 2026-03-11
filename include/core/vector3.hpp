#pragma once
#include "dual.hpp"

template <typename T>
struct Vector3 {
    T x, y, z;

    Vector3(T x=0, T y=0, T z=0) : x(x), y(y), z(z) {}

    inline Vector3 operator+(const Vector3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    inline Vector3 operator*(const T& scalar) const { return {x * scalar, y * scalar, z * scalar}; }
    
    inline T dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }
    inline Vector3 cross(const Vector3& v) const { return Vector3(y*v.z - v.y*z, z*v.x - v.z*x, x*v.y - v.x*y); }
};