#pragma once
#include "system_state.hpp"
#include "body_definition.hpp"

namespace DiffX {
    template <typename T>
    class Simulation {
        /*
        Architectural Purpose: Manages the timeline. It owns the BodyDefinitions and the history of SystemStates.
        Mathematical Responsibility: Orchestrating the Forward Pass. It seeds the initial "Dual" weights. For example, if you want to optimize "Initial Kick Force," you set kick_force.der = 1.0 before calling run()
        */

        std::vector<SystemState> tape; // The history for BPTT
        std::vector<BodyDefinition> params;

        void run_forward(int num_steps, double dt);
        double get_final_gradient(int body_idx); 
        void reset_to_state(const SystemState& start);
    };
}