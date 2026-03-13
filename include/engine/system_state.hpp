#pragma once
#include <vector>
#include "rigidbody.hpp"

namespace DiffX {
    template <typename T>
    class SystemState {
        /*
        Architectural Purpose: To act as a "Frame" in the simulation tape. It must be lightweight because you will store thousands of these for BPTT.
        Mathematical Responsibility: To provide the "Snapshot" St​. Each Dual value in this struct carries the accumulated partial derivative ∂θ∂St​​ relative to your input parameters.
        */

        public:
            std::vector<Rigidbody<T>> bodies;

            void add_body(Rigidbody<T> b) {
                b.ID = bodies.size();
                bodies.push_back(b);
            }

            SystemState<T>& operator=(const SystemState<T>& other) {
                if (this != &other) {
                    this->timestep = other.timestep;
                    this->bodies = other.bodies;
                }
                return *this;
            }
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const SystemState<T>& ss) {
        os << "System State\n";
        for (int i = 0; i < ss.bodies.size(); i++) {
            os << ss.bodies[i];
        }
        os << "\n";
        return os;
    }  
}