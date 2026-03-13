#include "raylib.h"
#include "core/dual.hpp"
#include "core/vector3.hpp"
#include "engine/simulation.hpp"
#include "engine/solver.hpp"
#include "engine/optimizer.hpp"
#include <iostream>
#include <vector>
#include <iomanip>

using namespace DiffX;

int main() {
    // init
    int fps = 60;
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(0, 0, "Differentiable Physics Engine - DiffX");
    SetTargetFPS(fps);

    Camera3D camera = { 0 };
    camera.position = (::Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target = (::Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (::Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    std::cout << std::fixed << std::setprecision(4);

    // params
    int total_iterations = 70;
    int interval = total_iterations/5;
    float target_e = 0.8f;
    float start_h = 2.0f;
    float ball_mass = 1.0f;
    Dual dissipation_guess(2000.0f, 1.0f);
    Dual k(5000.0f, 0.0f);
    float learning_rate = 100000.0f;
    
    BodyDefinition<Dual> ball_def(ball_mass);
    
    Rigidbody<Dual> initial_ball_state;
    initial_ball_state.body = ball_def;
    initial_ball_state.position = DiffX::Vector3<Dual>(0.0f, start_h, 0.0f);
    initial_ball_state.linear_velocity = DiffX::Vector3<Dual>(0.0f, 0.0f, 0.0f);
    
    // secondary params
    float dt = 1.0f/(float)fps;
    float target_h = start_h*target_e*target_e;
    DiffX::Vector3<Dual> gForce(0.0f, -9.81f*ball_mass, 0.0f);
    
    float last_loss = -1;
    float last_e = -1;
    for (int iter = 0; iter < total_iterations; iter++) {
        if (WindowShouldClose()) break;
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();

        // Optimizer<float> opt;
        // opt.learning_rate = 0.5f; 
        
        SystemState<Dual> initial_state;
        initial_state.add_body(initial_ball_state);
        Simulation<Dual> sim(initial_state);
        
        // sim
        bool hit_ground = false;
        bool going_up = false;
        bool imp = (iter%interval==0) ? true : false;
        DiffX::Dual max_height(0.0f, 0.0f);
        Rigidbody<Dual> *BallState = &sim.tape.back().bodies[0];

        while (hit_ground == false || going_up == true || BallState->position.y.v < 0.5f) {
            if (WindowShouldClose()) break;

            // calc force
            // BallState = &(sim.tape.back().bodies[0]);
            
            Dual activation = exp(-BallState->position.y / Dual(0.1f)); 
            Dual spring_f = activation * k;
            Dual damping_f = -BallState->linear_velocity.y * activation * dissipation_guess;
            DiffX::Vector3<Dual> total_f = DiffX::Vector3<Dual>(0.0f, spring_f + damping_f, 0.0f) + gForce;
            BallState->force = total_f;
            
            // std::cout << activation << "  " << spring_f << "  " << damping_f << "\n";
            // std::cout << sim.tape.back();

            // fwd
            sim.run_forward(1, dt);
            
            BallState = &(sim.tape.back().bodies[0]);
            
            if (BallState->linear_velocity.y.v < 0 && BallState->position.y.v < 0.5) hit_ground = true;
            if (BallState->linear_velocity.y.v > 0) going_up = true;
            if (BallState->linear_velocity.y.v < 0) going_up = false;
            if (hit_ground && BallState->position.y.v > max_height.v) {
                max_height = BallState->position.y;
            }

            if (imp) {
                BeginDrawing();
                    ClearBackground(BLACK);
                    float uiScale = GetScreenWidth() / 1920.0f;
                    if (uiScale < 0.7f) uiScale = 0.7f;

                    BeginMode3D(camera);
                        DrawGrid(10, 1.0f);
                        ::Vector3 rayPos = { BallState->position.x.v, BallState->position.y.v, BallState->position.z.v };
                        DrawSphere(rayPos, 0.5f, GOLD);
                    EndMode3D();

                    int panelWidth = 350 * uiScale;
                    int panelHeight = 260 * uiScale;
                    int padding = 15 * uiScale;

                    DrawRectangle(10, 10, panelWidth, panelHeight, Fade(BLACK, 0.8f));
                    DrawRectangleLines(10, 10, panelWidth, panelHeight, DARKGRAY);

                    DrawText(TextFormat("PHASE: %s", imp ? "VISUALIZING" : "CRUNCHING"), 10 + padding, 20, 22 * uiScale, imp ? GOLD : SKYBLUE);
                    DrawText(TextFormat("Iteration: %d / %d", iter, total_iterations), 10 + padding, 50 * uiScale, 18 * uiScale, RAYWHITE);

                    DrawLine(10 + padding, 80 * uiScale, 10 + panelWidth - padding, 80 * uiScale, DARKGRAY);
                    DrawText("TARGETS:", 10 + padding, 90 * uiScale, 15 * uiScale, GRAY);
                    DrawText(TextFormat("Target Height: %.4f m", target_h), 10 + padding, 110 * uiScale, 20 * uiScale, LIME);
                    DrawText(TextFormat("Target e:      %.4f", target_e), 10 + padding, 135 * uiScale, 20 * uiScale, GREEN);

                    DrawLine(10 + padding, 165 * uiScale, 10 + panelWidth - padding, 165 * uiScale, DARKGRAY);
                    DrawText("LIVE PERFORMANCE:", 10 + padding, 175 * uiScale, 15 * uiScale, GRAY);
                    DrawText(TextFormat("Current Peak:  %.4f m", max_height.v), 10 + padding, 195 * uiScale, 20 * uiScale, GOLD);
                    DrawText((last_e == -1) ? "Previous e: NA" : TextFormat("Previous e:     %.4f", last_e), 10 + padding, 225 * uiScale, 20 * uiScale, ORANGE);

                    int optWidth = 360 * uiScale;
                    int optHeight = 120 * uiScale;
                    int rightX = GetScreenWidth() - optWidth - 10;

                    DrawRectangle(rightX, 10, optWidth, optHeight, Fade(BLACK, 0.8f));
                    DrawRectangleLines(rightX, 10, optWidth, optHeight, DARKGRAY);
                    
                    DrawText("OPTIMIZER STATE:", rightX + padding, 20, 15 * uiScale, GRAY);
                    DrawText(TextFormat("Dissipation: %.4f", dissipation_guess), rightX + padding, 45 * uiScale, 20 * uiScale, SKYBLUE);
                    DrawText(TextFormat("Learn Rate:  %g", learning_rate), rightX + padding, 70 * uiScale, 20 * uiScale, YELLOW);
                    DrawText((last_loss == -1) ? "Prev Loss: NA" : TextFormat("Prev Loss:   %.6f", last_loss), rightX + padding, 95 * uiScale, 20 * uiScale, RED);

                    const char* hint = "Click F11 to Toggle Fullscreen";
                    int fontSize = 20 * uiScale;
                    int textWidth = MeasureText(hint, fontSize);
                    DrawText(hint, GetScreenWidth()/2 - textWidth/2, GetScreenHeight() - 40 * uiScale, fontSize, Fade(GRAY, 0.6f));

                EndDrawing();
            }
        }

        SystemState<Dual> final_state = sim.tape.back();
        Dual current_height = final_state.bodies[0].position.y;
        
        Dual loss = (current_height - target_h) * (current_height - target_h);
        last_loss = loss.v;
        last_e = sqrt(max_height.v/start_h);

        float gradient = loss.d;
        dissipation_guess.v -= gradient * learning_rate;
    }

    CloseWindow();

    std::cout << "\nLoss: " << last_loss << "    e: " << last_e << "\n";
    return 0;
}