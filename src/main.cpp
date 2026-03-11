#include "raylib.h"
#include "core/dual.hpp"
#include "core/vector3.hpp"
#include "engine/solver.hpp"
#include <iomanip>
#include <iostream>

// Configuration
float target_e = 0.7f; 
float start_h = 2.0f;
float target_bounce_h = start_h * (target_e * target_e);
float dissipation_guess = 2000.0f;
float learning_rate = 50000.0f;
int total_iterations = 1000;
int interval = total_iterations/10;

int main() {
    // 1. Setup Raylib
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "Differentiable Physics Training");
    SetTargetFPS(500); // Set high so training isn't too slow, or use 60 for smooth viewing

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    DiffX::Dual<float> k(5000.0f, 0.0f);
    DiffX::Vector3<DiffX::Dual<float>> g(0.0f, -9.81f, 0.0f);
    std::cout << std::fixed << std::setprecision(4);

    float last_loss = -1;
    float last_e = -1;
    for (int iter = 0; iter <= total_iterations; ++iter) {
        if (WindowShouldClose()) break;

        // Reset Simulation State for this Iteration
        DiffX::Dual<float> diss(dissipation_guess, 1.0f);
        DiffX::Vector3<DiffX::Dual<float>> pos(0.0f, start_h, 0.0f);
        DiffX::Vector3<DiffX::Dual<float>> vel(0.0f, 0.0f, 0.0f);
        DiffX::Dual<float> max_height(0.0f, 0.0f);
        
        DiffX::Solver<DiffX::Dual<float>> engine;
        float dt = 0.002f;

        bool hit_ground = false;
        bool going_up = false;
        bool imp = (iter%interval==0) ? true : false;
        // INNER LOOP: Single Physics Run (until peak is reached)
        while (hit_ground == false || going_up == true || pos.y.v < 0.5f) {
            if (WindowShouldClose()) break;

            engine.step(pos, vel, dt, g, k, diss);
            
            // Logic to track peak
            if (vel.y.v < 0 && pos.y.v < 0.5) hit_ground = true;
            if (vel.y.v > 0) going_up = true;
            if (vel.y.v < 0) going_up = false;
            if (hit_ground && pos.y.v > max_height.v) {
                max_height = pos.y;
            }

            if (imp) {
                BeginDrawing();
                    ClearBackground(BLACK);
                    BeginMode3D(camera);
                        DrawGrid(10, 1.0f);
                        // Map your Vector3 to Raylib Vector3
                        ::Vector3 rayPos = { pos.x.v, pos.y.v, pos.z.v };
                        DrawSphere(rayPos, 0.5f, GOLD);
                    EndMode3D();

                    // --- UI OVERLAY: Status & Progress ---
                    DrawRectangle(10, 10, 330, 240, Fade(BLACK, 0.8f)); // Semi-transparent background for readability
                    DrawRectangleLines(10, 10, 330, 240, DARKGRAY);

                    DrawText(TextFormat("PHASE: %s", imp ? "VISUALIZING" : "CRUNCHING"), 20, 20, 20, imp ? GOLD : SKYBLUE);
                    DrawText(TextFormat("Iteration: %d / %d", iter, total_iterations), 20, 45, 18, RAYWHITE);

                    DrawLine(20, 75, 320, 75, DARKGRAY); // Divider
                    DrawText("TARGETS:", 20, 85, 15, GRAY);
                    DrawText(TextFormat("Target Height: %.4f m", target_bounce_h), 20, 105, 20, LIME);
                    DrawText(TextFormat("Target e:      %.4f", target_e), 20, 130, 20, GREEN);

                    DrawLine(20, 160, 320, 160, DARKGRAY); // Divider
                    DrawText("LIVE PERFORMANCE:", 20, 170, 15, GRAY);
                    DrawText(TextFormat("Current Peak:  %.4f m", max_height.v), 20, 190, 20, GOLD);
                    DrawText((last_e==-1) ? "Previous e: NA" : TextFormat("Previous e:     %.4f", last_e), 20, 215, 20, ORANGE);

                    int rightX = GetScreenWidth() - 350;
                    DrawRectangle(rightX, 10, 340, 110, Fade(BLACK, 0.8f));
                    DrawText("OPTIMIZER STATE:", rightX + 10, 20, 15, GRAY);
                    DrawText(TextFormat("Dissipation (Guess): %.4f", dissipation_guess), rightX + 10, 40, 20, SKYBLUE);
                    DrawText(TextFormat("Learning Rate:       %g", learning_rate), rightX + 10, 65, 20, YELLOW);
                    DrawText((last_loss==-1) ? "Previous Loss: NA" : TextFormat("Previous Loss:        %.6f", last_loss), rightX + 10, 90, 20, RED);
                EndDrawing();
            }
        }

        // CALCULATION: Update based on the peak reached in the while loop
        DiffX::Dual<float> loss = (max_height - target_bounce_h) * (max_height - target_bounce_h);
        last_loss = loss.v;
        last_e = sqrt(max_height.v/start_h);
        dissipation_guess -= loss.d * learning_rate;

        if (imp) {
            std::cout << "Iter " << iter << " | Loss: " << loss.v << " | Dissipation: " << dissipation_guess << " | Peak H: " << max_height.v << " | e: " << last_e << std::endl;
        }
    }

    CloseWindow();
    return 0;
}