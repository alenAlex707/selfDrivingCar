#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "GeneticAlgorithm.h"
#include "Car.h"

using namespace std;

GeneticAlgo::GeneticAlgo() {}
GeneticAlgo::GeneticAlgo(int popSize, Vector2 spawn, float spawnAngle)
{
  this->populationSize = popSize;
  this->generation = 0;
  this->population = vector<Car>(popSize);
  this->eliteCount = 20;
  this->mutationRate = .25; // 50% chance
  this->mutationStrength = 1.5;

  for (auto &car : population)
  {
    car.spawn = spawn;
    car.angle = spawnAngle;
    car.spawnAngle = spawnAngle;
    car.x = spawn.x;
    car.y = spawn.y;
  }
}

int GeneticAlgo::aliveCars()
{
  int c{};
  for (auto &car : population)
  {
    if (car.alive)
    {
      c++;
    }
  }
  return c;
}

bool GeneticAlgo::allDead()
{
  if (population.empty())
    return false;

  for (auto &car : population)
  {
    if (car.alive == true)
    {
      return false;
    }
  }
  return true;
}

Car GeneticAlgo::createChild()
{
  int rnd1 = rand() % 20;
  int rnd2 = rand() % 20;
  int cutpoint = rand() % 50;

  Car child;

  vector<float> crossover;
  for (int i = 0; i < 50; i++)
  {
    if (i < cutpoint)
    {
      crossover.push_back(eliteWeights[rnd1][i]);
    }
    else
    {
      crossover.push_back(eliteWeights[rnd2][i]);
    }
  }

  for (auto &val : crossover)
  {
    float mutationChance = (rand() % 101) / 100.0f;
    float nudge = -mutationStrength + (rand() / (float)RAND_MAX) * (2 * mutationStrength);

    if (mutationChance < mutationRate)
    {
      val += nudge;
    }
  }

  child.brain.setWeights(crossover);
  return child;
}

void GeneticAlgo::evolve()
{
  eliteWeights.clear();
  sort(population.begin(), population.end(), [](const Car &a, const Car &b)
       { return a.fitness > b.fitness; });

  for (int i = 0; i < eliteCount; i++)
  {
    eliteWeights.push_back(population[i].brain.getWeights());
  }

  for (int i = eliteCount; i < populationSize; i++)
  {
    population[i] = createChild();
    population[i].spawn = population[0].spawn;
    population[i].spawnAngle = population[0].spawnAngle;
  }

  for (auto &car : population)
  {
    car.reset();
  }
  generation++;
}