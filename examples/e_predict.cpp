#include <iostream>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include "core/dual.hpp"
#include "core/vector3.hpp"
#include "engine/solver.hpp"

int main() {
    float target_e =0.96f; 
    float start_h = 2.0f;
    float target_bounce_h = start_h * (target_e * target_e);
    
    float dissipation_guess = 5.0f;
    float learning_rate = 10000.0f;
    std::cout << std::fixed << std::setprecision(4);

    std::cout << "Initial Height: " << start_h << "m" << std::endl << "Target Height: " << target_bounce_h << std::endl << "e: " << target_e << std::endl;

    DiffX::Dual<float> k(5000.0f, 0.0f);
    DiffX::Vector3<DiffX::Dual<float>> g(0.0f, -9.81f, 0.0f);

    for (int iter = 0; iter < 1000; ++iter) {
        DiffX::Dual<float> diss(dissipation_guess, 1.0f);

        DiffX::Vector3<DiffX::Dual<float>> pos(0.0f, start_h, 0.0f);
        DiffX::Vector3<DiffX::Dual<float>> vel(0.0f, 0.0f, 0.0f);
        DiffX::Dual<float> max_height(0.0f, 0.0f);
        
        DiffX::Solver<DiffX::Dual<float>> engine;
        float dt = 0.001f;

        bool hit_ground = false;
        bool going_up = false;
        while (hit_ground == false || going_up == true || pos.y.v < 0.5f) {
            engine.step(pos, vel, dt, g, k, diss);
            
            if (vel.y.v < 0 && pos.y.v < 0.5) hit_ground = true;
            if (vel.y.v > 0) going_up = true;
            if (vel.y.v < 0) going_up = false;
            if (hit_ground && pos.y.v > max_height.v) {
                max_height = pos.y;
            }
        }

        DiffX::Dual<float> loss = (max_height - target_bounce_h) * (max_height - target_bounce_h);

        dissipation_guess -= loss.d * learning_rate;

        std::cout << "Iter " << iter << " | Dissipation: " << dissipation_guess << " | Peak H: " << max_height.v << " | e: " << sqrt(max_height.v/start_h) << std::endl;
    }

    return 0;
}