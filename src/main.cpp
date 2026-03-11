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
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(0, 0, "Differentiable Physics Engine - DiffX");
    SetTargetFPS(1000);

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
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();

        DiffX::Dual<float> diss(dissipation_guess, 1.0f);
        DiffX::Vector3<DiffX::Dual<float>> pos(0.0f, start_h, 0.0f);
        DiffX::Vector3<DiffX::Dual<float>> vel(0.0f, 0.0f, 0.0f);
        DiffX::Dual<float> max_height(0.0f, 0.0f);
        
        DiffX::Solver<DiffX::Dual<float>> engine;
        float dt = 0.002f;

        bool hit_ground = false;
        bool going_up = false;
        bool imp = (iter%interval==0) ? true : false;
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
                    float uiScale = GetScreenWidth() / 1920.0f;
                    if (uiScale < 0.7f) uiScale = 0.7f;

                    BeginMode3D(camera);
                        DrawGrid(10, 1.0f);
                        ::Vector3 rayPos = { pos.x.v, pos.y.v, pos.z.v };
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
                    DrawText(TextFormat("Target Height: %.4f m", target_bounce_h), 10 + padding, 110 * uiScale, 20 * uiScale, LIME);
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