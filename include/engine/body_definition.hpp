#pragma once
#include "matrix3x3.hpp"

namespace DiffX
{
    template <typename T>
    struct BodyDefinition {
        T mass;
        Matrix3x3<T> inertia_tensor;
        T restitution;
    };
}
