#pragma once
#include <cmath>

namespace DiffX {
    template <typename T>
    struct Quaternion {
        T x, y, z, w;
        
        Quaternion() : x(0), y(0), z(0), w(0) {}
        Quaternion(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

        Quaternion operator *(Quaternion a, Quaternion b) {
            return Quaternion(
                a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
                a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
                a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
                a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
            );
        }

        Quaternion conjugate() const {
            return Quaternion(-x, -y, -z, w);
        }

        T norm_squared() const {
            return x * x + y * y + z * z + w * w;
        }
    };
}