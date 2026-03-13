#pragma once
#include <cmath>

namespace DiffX {
    template <typename T>
    struct Quaternion {
        T x, y, z, w;
        
        Quaternion() : x(0), y(0), z(0), w(0) {}
        Quaternion(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

        Quaternion operator *(Quaternion b) {
            return Quaternion(
                w * b.x + x * b.w + y * b.z - z * b.y,
                w * b.y + y * b.w + z * b.x - x * b.z,
                w * b.z + z * b.w + x * b.y - y * b.x,
                w * b.w - x * b.x - y * b.y - z * b.z
            );
        }

        Quaternion conjugate() const {
            return Quaternion(-x, -y, -z, w);
        }

        T norm_squared() const {
            return x * x + y * y + z * z + w * w;
        }
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Quaternion<T>& q) {
        os << "Q: " << q.w << " | [  " << q.x << ", " << q.y << ", " << q.z << "  ]";
        return os;
    }
}