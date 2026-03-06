#include "raylib.h"
#include <iostream>
#include <math.h>
using namespace std;

class Car
{
public:
    float x;
    float y;

    float width;
    float height;

    float angle;
    float rotationSpeed;

    float vel_x;
    float vel_y;

    float speed;
    float acceleration;
    float retardation;

    Vector2 origin;

    Car(float x, float y)
    {
        this->x = x;
        this->y = y;

        angle = 0.0f;
        rotationSpeed = 180.0f;

        vel_x = 0.0f;
        vel_y = 0.0f;

        speed = 0.0f;
        acceleration = 2500.0f;
        retardation = -2.0f;

        width = 40;
        height = 20;
        origin = {width / 2, height / 2};
    }

    void Update(float dt)
    {
        float rad = angle * DEG2RAD;

        // input
        if (IsKeyDown(KEY_W))
            speed += acceleration * dt;
        if (IsKeyDown(KEY_A))
            angle -= rotationSpeed * dt;
        if (IsKeyDown(KEY_S))
            speed -= acceleration * dt;
        if (IsKeyDown(KEY_D))
            angle += rotationSpeed * dt;
        if (IsKeyDown(KEY_SPACE))
            speed += speed * retardation * dt * 1.4;

        if (fabs(speed) < 5.0f)
            speed = 0;

        speed += speed * retardation * dt;

        vel_x = cos(rad) * speed;
        vel_y = sin(rad) * speed;

        x += vel_x * dt;
        y += vel_y * dt;
    }

    void Draw()
    {
        Rectangle rect = {x, y, width, height};
        DrawRectanglePro(rect, origin, angle, RED);
        rect.x = x;
        rect.y = y;
    }
};

int main()
{
    InitWindow(1920, 1080, "test window");
    SetTargetFPS(60);

    Car car(960, 540);
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        car.Update(dt);

        BeginDrawing();
        ClearBackground(BLACK);
        car.Draw();
        DrawText(TextFormat("X: %.2f", car.x), 20, 20, 20, WHITE);
        DrawText(TextFormat("Y: %.2f", car.y), 20, 45, 20, WHITE);
        DrawText(TextFormat("Speed: %.2f", car.speed), 20, 70, 20, WHITE);
        DrawText(TextFormat("Angle: %.2f", car.angle), 20, 95, 20, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}