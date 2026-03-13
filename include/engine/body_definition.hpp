#pragma once
#include "core/matrix3x3.hpp"

namespace DiffX
{
    template <typename T>
    struct BodyDefinition {
        T mass;
        T invMass;
        Matrix3x3<T> inertia_tensor;
        T restitution;

        BodyDefinition<T>() : mass(1.0f) {}
        BodyDefinition<T>(float m) : mass(m), invMass(1.0f/m) {}
    };
}
