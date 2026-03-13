#pragma once
#include "system_state.hpp"
#include "body_definition.hpp"
#include "system_state.hpp"
#include "solver.hpp"
#include "core/dual.hpp"

namespace DiffX {
    template <typename T>
    class Simulation {
        /*
        Architectural Purpose: Manages the timeline. It owns the BodyDefinitions and the history of SystemStates.
        Mathematical Responsibility: Orchestrating the Forward Pass. It seeds the initial "Dual" weights. For example, if you want to optimize "Initial Kick Force," you set kick_force.der = 1.0 before calling run()
        */

       DiffX::Solver<T> solver;
       
       public:
       std::vector<SystemState<T>> tape;
       std::vector<BodyDefinition<T>> params;
       
        void run_forward(int num_steps, float dt) {
            for (int i = 0; i < num_steps; i++) {
                SystemState<T> state = solver.step(tape.back(), dt);
                tape.push_back(state);
            }
        }

        void clear() {
            tape.clear();
        }

        Simulation<T>(const SystemState<T> initial) {
            tape.push_back(initial);
        }
    };
}