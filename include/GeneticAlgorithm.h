#pragma once

#include "Car.h"
#include <vector>

using namespace std;

class GeneticAlgo
{
public:
  vector<Car> population;
  Vector2 localSpawn;
  int generation;
  int populationSize;
  int eliteCount;
  vector<vector<float>> eliteWeights;
  float bestFitnessEver;
  int stuckGenerations;
  float mutationRate;
  float mutationStrength;
  GeneticAlgo();
  GeneticAlgo(int popSize, Vector2 spawn, float spawnAngle);
  int aliveCars();
  bool allDead();
  Car createChild();
  void evolve();
};