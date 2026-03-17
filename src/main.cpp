#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <iostream>

#include "Car.h"
#include "Utils.h"

using namespace std;

int main()
{
  srand(time(0));
  InitWindow(1920, 1080, "test window");
  SetTargetFPS(120);

  Car car(960, 540);

  vector<Vector2> path;
  vector<Wall> walls;

  Vector2 spawnpoint;

  Rectangle setTrackBtn = {1750, 1000, 120, 40};

  bool canDrawPath = true;
  bool trackSet = false;

  while (!WindowShouldClose())
  {
    float dt = GetFrameTime();
    car.Update(trackSet, dt, walls);

    if (canDrawPath)
    {
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsKeyDown(KEY_LEFT_SHIFT))
      {
        Vector2 mouse = GetMousePosition();
        path.push_back(mouse);
      }
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
      path.clear();
      walls.clear();
      trackSet = false;
      canDrawPath = true;
      cout << "cleared" << endl;
    }

    BeginDrawing();
    ClearBackground({20, 20, 20, 255});

    for (size_t i = 0; i + 1 < path.size(); i++)
    {
      DrawLineV(path[i], path[i + 1], LIGHTGRAY);
      DrawCircle(path[i].x, path[i].y, 5, LIGHTGRAY);
    }

    if (trackSet)
    {
      DrawCircle(path[path.size() - 1].x, path[path.size() - 1].y, 5, WHITE);
    }

    car.Draw(trackSet);

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

    if (buttonClicked(setTrackBtn) && trackSet == false && path.size() != 0)
    {
      trackSet = true;
      canDrawPath = false;

      spawnpoint.x = (path[0].x + path[path.size() - 1].x) / 2;
      spawnpoint.y = (path[0].y + path[path.size() - 1].y) / 2;

      car.x = spawnpoint.x;
      car.y = spawnpoint.y;

      for (size_t i = 0; i + 1 < path.size(); i++)
      {
        walls.push_back({path[i], path[i + 1]});
      }
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}