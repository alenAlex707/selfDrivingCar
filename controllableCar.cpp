#include "raylib.h"
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

struct Wall
{
  Vector2 a;
  Vector2 b;
};

Vector2 getIntersection(Wall wall, Vector2 sensorStart, Vector2 sensorEnd)
{
  Vector2 d1 = {(wall.b.x - wall.a.x), (wall.b.y - wall.a.y)};
  Vector2 d2 = {(sensorEnd.x - sensorStart.x), (sensorEnd.y - sensorStart.y)};

  float t, s;
  t = ((sensorStart.x - wall.a.x) * d2.y - (sensorStart.y - wall.a.y) * d2.x) / ((d1.x * d2.y) - (d1.y * d2.x));
  s = ((sensorStart.x - wall.a.x) * d1.y - (sensorStart.y - wall.a.y) * d1.x) / ((d1.x * d2.y) - (d1.y * d2.x));

  Vector2 intersection;

  if (t < 1 && t > 0 && s < 1 && s > 0)
  {
    intersection.x = wall.a.x + t * d1.x;
    intersection.y = wall.a.y + t * d1.y;
  }

  return {intersection};
}

bool buttonClicked(Rectangle rec)
{
  Vector2 mouse = GetMousePosition();
  if (CheckCollisionPointRec(mouse, rec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
  {
    return true;
  }
  return false;
}

bool buttonHover(Rectangle rec)
{
  Vector2 mouse = GetMousePosition();
  if (CheckCollisionPointRec(mouse, rec))
  {
    return true;
  }
  return false;
}

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

  float sensorLength;
  int sensorCount;
  vector<float> sensorAngles;

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
    acceleration = 1500.0f;
    retardation = -3.5f;

    width = 40;
    height = 20;
    origin = {width / 2, height / 2};

    sensorLength = 200;
    sensorCount = 5;
    sensorAngles = {-45, -20, 0, 20, 45};
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

    for (int i = 0; i < sensorCount; i++)
    {
      float sensorAngle = (angle + sensorAngles[i]) * DEG2RAD;
      Vector2 sensorStart = {x, y};
      Vector2 sensorEnd = {x + cos(sensorAngle) * sensorLength, y + sin(sensorAngle) * sensorLength};

      DrawLine(sensorStart.x, sensorStart.y, sensorEnd.x, sensorEnd.y, YELLOW);
    }
  }
};

int main()
{
  InitWindow(1920, 1080, "test window");
  SetTargetFPS(60);

  Car car(960, 540);

  vector<Vector2> path;
  vector<Wall> walls;

  Rectangle setTrackBtn = {1750, 1000, 120, 40};
  bool trackSet = false;

  while (!WindowShouldClose())
  {
    float dt = GetFrameTime();
    car.Update(dt);

    // path mouse input
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsKeyDown(KEY_LEFT_SHIFT))
    {
      Vector2 mouse = GetMousePosition();
      path.push_back(mouse);
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
      path.clear();
      walls.clear();
      trackSet = false;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    // path drawing
    for (size_t i = 0; i + 1 < path.size(); i++)
    {
      DrawLineV(path[i], path[i + 1], WHITE);
      DrawCircle(path[i].x, path[i].y, 5, WHITE);
    }

    // draw car
    car.Draw();

    // HUD
    DrawText(TextFormat("X: %.2f", car.x), 20, 20, 20, GREEN);
    DrawText(TextFormat("Y: %.2f", car.y), 20, 45, 20, GREEN);
    DrawText(TextFormat("Speed: %.2f", car.speed), 20, 70, 20, GREEN);
    DrawText(TextFormat("Angle: %.2f", car.angle), 20, 95, 20, GREEN);

    DrawRectangleRec(setTrackBtn, GRAY);
    DrawText("Set Track", 1755, 1005, 20, BLACK);

    if (buttonHover(setTrackBtn))
    {
      DrawRectangleRec(setTrackBtn, DARKGRAY);
      DrawText("Set Track", 1755, 1005, 20, BLACK);
    }
    if (buttonClicked(setTrackBtn) && trackSet == false)
    {
      trackSet = true;
      cout << "clicked" << endl;
      for (size_t i = 0; i + 1 < path.size(); i++)
      {
        walls.push_back({path[i], path[i + 1]});
      }

      for (Wall k : walls)
      {
        cout << k.a.x << "," << k.a.y << "  " << k.b.x << "," << k.b.y << endl;
      }
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}