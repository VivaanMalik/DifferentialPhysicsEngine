#pragma once
#include "core/vector3.hpp"
#include "core/quaternion.hpp"
#include "body_definition.hpp"

namespace DiffX {
    template <typename T>
    struct  Rigidbody
    {
        public:
        int ID;
        DiffX::BodyDefinition<T> body;
        DiffX::Vector3<T> position;
        DiffX::Quaternion<T> orientation;
        DiffX::Vector3<T> linear_velocity;
        DiffX::Vector3<T> angular_velocity;
        DiffX::Vector3<T> force;

        Rigidbody<T>() {}
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Rigidbody<T>& b) {
        os << "  Body: ID[" << b.ID << "]\n";
        os << "    Mass:     " << b.body.mass << "\n";
        os << "    Position: " << b.position << "\n";
        os << "    Velocity: " << b.linear_velocity << "\n";
        os << "    Force:    " << b.force << "\n";
        return os;
    }    
}