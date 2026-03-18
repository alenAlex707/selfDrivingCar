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
  this->mutationRate = 0.05; // 5% chance
  this->mutationStrength = 0.3;

  for (auto &car : population)
  {
    car.spawn = spawn;
    car.angle = spawnAngle;
    car.spawnAngle = spawnAngle;
    car.x = spawn.x;
    car.y = spawn.y;
  }
}

bool GeneticAlgo::allDead()
{
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
  cout << child.spawn.x << "," << child.spawn.y << endl;

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

  cout << generation << endl;
  generation++;
}