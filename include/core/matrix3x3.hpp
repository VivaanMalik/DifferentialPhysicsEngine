#pragma once
#include <cmath>
#include <initializer_list>
#include "vector3.hpp"

namespace DiffX {
    template <typename T>
    struct Matrix3x3 {
        T data[3][3];

        Matrix3x3() : data{{T(1.0f), T(0.0f), T(0.0f)}, {T(0.0f), T(1.0f), T(0.0f)}, {T(0.0f), T(0.0f), T(1.0f)}} {}

        Matrix3x3(std::initializer_list<std::initializer_list<T>> list) {
            int row = 0;
            for (auto& r : list) {
                int col = 0;
                for (auto& val : r) {
                    if (row < 3 && col < 3) data[row][col] = val;
                    col++;
                }
                row++;
            }
        }

        Vector3<T> operator*(const Vector3<T>& v) {
            return Vector3<T>(
                data[0][0]*v.x+data[0][1]*v.y+data[0][2]*v.z,
                data[1][0]*v.x+data[1][1]*v.y+data[1][2]*v.z,
                data[2][0]*v.x+data[2][1]*v.y+data[2][2]*v.z
            );
        }

        Matrix3x3<T> transpose() const {
            return Matrix3x3<T>{
                {data[0][0], data[1][0], data[2][0]},
                {data[0][1], data[1][1], data[2][1]},
                {data[0][2], data[1][2], data[2][2]}
            };
        }

        Matrix3x3<T> inverse() const {
            T m00 = data[1][1] * data[2][2] - data[1][2] * data[2][1];
            T m01 = data[0][2] * data[2][1] - data[0][1] * data[2][2];
            T m02 = data[0][1] * data[1][2] - data[0][2] * data[1][1];
            
            T m10 = data[1][2] * data[2][0] - data[1][0] * data[2][2];
            T m11 = data[0][0] * data[2][2] - data[0][2] * data[2][0];
            T m12 = data[0][2] * data[1][0] - data[0][0] * data[1][2];
            
            T m20 = data[1][0] * data[2][1] - data[1][1] * data[2][0];
            T m21 = data[0][1] * data[2][0] - data[0][0] * data[2][1];
            T m22 = data[0][0] * data[1][1] - data[0][1] * data[1][0];

            T det = data[0][0] * m00 + data[0][1] * m10 + data[0][2] * m20;

            T invDet = T(1.0) / det;

            return Matrix3x3<T>{
                {m00 * invDet, m01 * invDet, m02 * invDet},
                {m10 * invDet, m11 * invDet, m12 * invDet},
                {m20 * invDet, m21 * invDet, m22 * invDet}
            };
        }
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Matrix3x3<T>& m) {
        os << "⎡  " << m.data[0][0] << " " << m.data[0][1] << " " << m.data[0][2] << "  ⎤\n"
           << "⎢  " << m.data[0][0] << " " << m.data[0][1] << " " << m.data[0][2] << "  ⎥\n"
           << "⎣  " << m.data[0][0] << " " << m.data[0][1] << " " << m.data[0][2] << "  ⎦";
        return os;
    }
}