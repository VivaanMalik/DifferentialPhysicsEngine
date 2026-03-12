#pragma once
#include "vector3.hpp"
#include "dual.hpp"
#include "quaternion.hpp"

namespace DiffX {
    template <typename T>
    struct  Rigidbody
    {
        DiffX::Vector3<DiffX::Dual> position;
        DiffX::Quaternion<DiffX::Dual> position;
        DiffX::Vector3<DiffX::Dual> linear_velocity;
        DiffX::Vector3<DiffX::Dual> angular_velocity;
    };
    
}