#pragma once
#include "simulation.hpp"
#include "core/vector3.hpp"

namespace DiffX {
    template <typename T>
    class Optimizer {
        public:
            double learning_rate = 0.01;

            // The core loop
            void optimize_step(Simulation& sim, Vector3<double> target_pos) {
                // sim.run_forward(...);
                // Dual loss = calculate_loss(sim.tape.back(), target_pos);
                // double grad = loss.der;
                // sim.params[0].mass.val -= learning_rate * grad;
            }
    };
}