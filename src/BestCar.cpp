#include "BestCar.h"
#include <math.h>
#include "Utils.h"

BestCar::BestCar()
{
}

void BestCar::Update(bool trackSet, float dt, const vector<Wall> &walls, const Wall &finishLine)
{
  if (!trackSet)
    return;

  float rad = angle * DEG2RAD;
  vector<float> neuralOut = brain.forward(sensor.sensorValues);

  angle += neuralOut[0] * rotationSpeed * dt;
  speed += neuralOut[1] * acceleration * dt;
  speed += speed * retardation * dt;
  if (speed < 0)
    speed = 0;

  vel_x = cos(rad) * speed;
  vel_y = sin(rad) * speed;

  Vector2 oldPos = {x, y};
  x += vel_x * dt;
  y += vel_y * dt;
  sensor.UpdateVal(x, y, angle, walls);

  Vector2 cross = getIntersection(finishLine, oldPos, {x + cos(angle * DEG2RAD) * 25.0f, y + sin(angle * DEG2RAD) * 25.0f});
  if (cross.x != -1 && cross.y != -1)
  {
    reset();
  }
}