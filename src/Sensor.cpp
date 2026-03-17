#include "Sensor.h"
#include <math.h>
#include <iostream>

using namespace std;

Sensor::Sensor()
{
  sensorLength = 200;
  sensorAngles = {-45, -20, 0, 20, 45};
  sensorCount = sensorAngles.size();
  sensorValues = {0, 0, 0, 0, 0};
}

void Sensor::UpdateVal(float x, float y, float theta, const vector<Wall> &walls)
{
  this->carX = x;
  this->carY = y;
  this->carAngle = theta;

  this->walls = walls;
}

void Sensor::DrawSensor(bool trackSet)
{
  for (int i = 0; i < sensorAngles.size(); i++)
  {
    Vector2 intersection;
    float sensorAngle = (carAngle + sensorAngles[i]) * DEG2RAD;
    Vector2 sensorStart = {carX, carY};
    Vector2 sensorEnd = {carX + cos(sensorAngle) * sensorLength, carY + sin(sensorAngle) * sensorLength};

    float closestDist = INFINITY;
    Vector2 closestPoint = sensorEnd;
    bool hit = false;

    for (int j = 0; j < walls.size(); j++)
    {
      intersection = getIntersection(walls[j], sensorStart, sensorEnd);

      if (intersection.x != -1 && intersection.y != -1)
      {
        float distance = getDistance(sensorStart, intersection);
        if (distance < closestDist)
        {
          closestDist = distance;
          closestPoint = intersection;
          hit = true;
          // printf("INTERSECTION sensor %d: %.2f, %.2f \n", i + 1, intersection.x, intersection.y);
        }
      }
    }

    if (trackSet)
    {
      if (hit)
      {
        sensorValues[i] = closestDist / sensorLength;
        // cout << closestDist / sensorLength << endl;
        DrawLineV(sensorStart, closestPoint, {255, 40, 40, 255});
      }
      else
      {
        sensorValues[i] = 1;
        DrawLineV(sensorStart, sensorEnd, {255, 220, 0, 255});
      }
    }
    else
    {
      if (i == 2)
        DrawLineV(sensorStart, sensorEnd, BLUE);
    }
  }
}