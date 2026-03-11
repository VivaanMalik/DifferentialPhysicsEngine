#pragma once
#include "core/vector3.hpp"
#include <cmath>

template <typename T>
class Solver {
    public:
        float eps = 0.1f;

        // simulating springy
        void step(Vector3<T>& pos, Vector3<T>& vel, float dt, Vector3<T> gravity, T k, T dissipation) {        
            T activation = exp(-pos.y / T(eps)); 
            T spring_f = activation * k;
            T damping_f = -vel.y * activation * dissipation;

            Vector3<T> total_f = gravity + Vector3<T>(0.0f, spring_f + damping_f, 0.0f);

            vel = vel + total_f * T(dt);
            pos = pos + vel * T(dt);
        }
};