#pragma once
#include "Car.h"

class BestCar : public Car
{
public:
  BestCar();
  void Update(bool trackSet, float dt, const vector<Wall> &walls, const Wall &finishLine);
};