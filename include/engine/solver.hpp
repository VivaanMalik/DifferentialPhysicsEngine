#pragma once
#include <cmath>
#include <vector>
#include "core/vector3.hpp"
#include "body_definition.hpp"
#include "system_state.hpp"
#include "rigidbody.hpp"

namespace DiffX {
    template <typename T>
    class Solver {
        public:
            // simulating springy
            SystemState<T> step(SystemState<T> systemState, float dt) { 
                SystemState<T> newState = systemState;
                for (int b = 0; b < systemState.bodies.size(); b++) {    

                    newState.bodies[b].linear_velocity = systemState.bodies[b].linear_velocity + systemState.bodies[b].force*systemState.bodies[b].body.invMass * T(dt);
                    newState.bodies[b].position = systemState.bodies[b].position + systemState.bodies[b].linear_velocity * T(dt);

                    newState.bodies[b].force = {0, 0, 0};
                }
                return newState;
            }
    };
}