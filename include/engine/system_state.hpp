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
            double timestep;
            std::vector<Rigidbody> bodies;

            // helper
            Dual calculate_distance_to(int body_idx, Vector3<double> target);
    };
}