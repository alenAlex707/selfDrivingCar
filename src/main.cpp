#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <iostream>

#include "Car.h"
#include "GeneticAlgorithm.h"
#include "Utils.h"
#include "Button.h"

using namespace std;

int main()
{
  srand(time(0));
  InitWindow(1920, 1080, "test window");
  SetTargetFPS(120);

  vector<Vector2> path;
  vector<Wall> walls;

  Wall finishLine;
  bool finishLineSet = false;

  Vector2 spawnpoint;

  // button def area
  Button setTrackBtn({1750, 1000, 120, 40}, "Set track", 20);
  Button copyPath({1600, 1000, 120, 40}, "Copy track", 20);
  Button inputTrack({1450, 1000, 120, 40}, "Input track", 20);

  bool canDrawPath = true;
  bool trackSet = false;

  GeneticAlgo ga;

  while (!WindowShouldClose())
  {
    float dt = GetFrameTime();

    if (ga.allDead() && trackSet)
    {
      ga.evolve();
    }

    // only update if car is alive
    for (auto &car : ga.population)
      if (car.alive)
        car.Update(trackSet, dt, walls, finishLine);
    
    if (canDrawPath)
    {
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsKeyDown(KEY_LEFT_SHIFT))
      {
        Vector2 mouse = GetMousePosition();
        path.push_back(mouse);
      }
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_F) && finishLineSet == false && path.size() > 2)
      {
        finishLine.b = GetMousePosition();
        finishLineSet = true;

        for (int i = 0; i + 1 < path.size(); i++)
          if (path[i + 1].x == finishLine.b.x && path[i + 1].y == finishLine.b.y)
            finishLine.a = path[i];
      }
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
      path.clear();
      walls.clear();
      trackSet = false;
      canDrawPath = true;
      finishLineSet = false;
      cout << "cleared" << endl;
    }

    BeginDrawing();
    ClearBackground({20, 20, 20, 255});

    // path drawing
    for (size_t i = 0; i + 1 < path.size(); i++)
    {
      DrawLineV(path[i], path[i + 1], LIGHTGRAY);
      DrawCircle(path[i].x, path[i].y, 5, LIGHTGRAY);
    }

    if (finishLineSet)
      DrawLineV(finishLine.a, finishLine.b, PURPLE);
    // final dot drawing on path
    if (trackSet)
    {
      DrawCircle(path[path.size() - 1].x, path[path.size() - 1].y, 5, WHITE);
    }

    // only draw if car is alive
    for (auto &car : ga.population)
      if (car.alive)
        car.Draw(trackSet);

    DrawText("HUD", 20, 20, 20, GREEN);
    if (!trackSet)
    {
      DrawText(TextFormat("Generation: ", ga.generation), 20, 45, 20, GREEN);
    }
    else
    {
      DrawText(TextFormat("Alive cars: %.d", ga.aliveCars()), 20, 70, 20, GREEN);
      DrawText(TextFormat("Generation: %d", ga.generation + 1), 20, 45, 20, GREEN);
    }

    // DrawText(TextFormat("X: %.2f", car.x), 20, 20, 20, GREEN);
    // DrawText(TextFormat("Y: %.2f", car.y), 20, 45, 20, GREEN);
    // DrawText(TextFormat("Speed: %.2f", car.speed), 20, 70, 20, GREEN);
    // DrawText(TextFormat("Angle: %.2f", car.angle), 20, 95, 20, GREEN);

    setTrackBtn.draw();
    copyPath.draw();
    inputTrack.draw();

    if (setTrackBtn.buttonClicked() && trackSet == false && path.size() != 0)
    {
      trackSet = true;
      canDrawPath = false;

      spawnpoint.x = (path[0].x + path[path.size() - 1].x) / 2;
      spawnpoint.y = (path[0].y + path[path.size() - 1].y) / 2;

      ga = GeneticAlgo(100, spawnpoint, getStartAngle(path));

      for (size_t i = 0; i + 1 < path.size(); i++)
      {
        walls.push_back({path[i], path[i + 1]});
      }
    }

    if (copyPath.buttonClicked())
    {
      for (int i = 0; i < path.size(); i++)
      {
        cout << path[i].x << " " << path[i].y << " ";
      }
    }

    if (inputTrack.buttonClicked() && !trackSet)
    {
      vector<int> v;
      int x;

      cout << endl;
      cout << "enter path coords: " << endl;
      while (cin >> x)
      {
        v.push_back(x);

        if (cin.peek() == '\n')
          break;
      }

      path.clear();
      vector<Vector2> actualPath(v.size() / 2);

      for (int i = 0; i < v.size(); i += 2)
      {
        actualPath[i / 2].x = v[i];
        actualPath[i / 2].y = v[i + 1];
      }

      path = actualPath;
      cout << "path created!" << endl;

      actualPath.clear();
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}