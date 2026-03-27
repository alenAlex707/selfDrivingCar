# Self-Driving Car Simulation
A self-driving car simulation built from scratch in C++ using raylib. Features a custom neural network and genetic algorithm — no ML libraries, no backpropagation. Everything is hand-rolled.

---

## Overview

Cars learn to navigate a user-drawn track through neuroevolution. Each car has a small feedforward neural network as its brain. A genetic algorithm evolves the population across generations — selecting the best performers, breeding their weights, and mutating offspring. No gradient descent. No training data. Just survival of the fittest.

---

## Architecture

### Neural Network — `NeuralNetwork.h / .cpp`

Fixed topology: **5 inputs → 6 hidden → 2 outputs**

```
Input layer:   5 neurons  (sensor distances, normalized 0..1)
Hidden layer:  6 neurons  (tanh activation)
Output layer:  2 neurons  (steer, speed — tanh, range -1..+1)
```

**Total weights:** `(5×6) + (6×2) + 6 + 2 = 50 floats`

The hidden layer uses tanh because its -1..+1 output range naturally maps to directional decisions. Each neuron computes:

```
sum = bias + Σ(input[i] × weight[i])
output = tanh(sum)
```

**Forward pass:**

```cpp
vector<float> forward(const vector<float>& inputs) {
    vector<float> hidden(HIDDEN_N);
    for (int h = 0; h < HIDDEN_N; h++) {
        float sum = bias_h[h];
        for (int i = 0; i < INPUT_N; i++)
            sum += inputs[i] * weights_ih[i][h];
        hidden[h] = tanh(sum);
    }

    vector<float> outputs(OUTPUT_N);
    for (int o = 0; o < OUTPUT_N; o++) {
        float sum = bias_o[o];
        for (int h = 0; h < HIDDEN_N; h++)
            sum += hidden[h] * weights_ho[h][o];
        outputs[o] = tanh(sum);
    }
    return outputs;
}
```

**Weight serialization** — the GA treats the entire brain as a flat vector of 50 floats for crossover and mutation:

```cpp
vector<float> getWeights();       // flatten: weights_ih → weights_ho → bias_h → bias_o
void setWeights(vector<float>);   // unpack back in the same order
```

---

### Sensors — `Sensor.h / .cpp`

5 raycasts fired from the car's center at fixed angles:

```
Angles: -45°, -20°, 0°, +20°, +45°
        left  fl   front  fr   right
```

Each raycast finds the closest wall intersection and returns a normalized distance:

```cpp
sensorValues[i] = closestDist / sensorLength;  // 0.0 = wall right here, 1.0 = nothing
```

Collision detection uses the same raycast math — if any sensor value drops below `0.05f`, the car is considered crashed.

---

### Car — `Car.h / .cpp`

Each car owns a `NeuralNetwork brain`. Every frame:

```cpp
void Car::Update(bool trackSet, float dt, const vector<Wall>& walls, const Wall& finishLine) {
    vector<float> neuralOut = brain.forward(sensor.sensorValues);

    angle += neuralOut[0] * rotationSpeed * dt;   // steer
    speed += neuralOut[1] * acceleration * dt;    // throttle
    speed += speed * retardation * dt;            // drag

    if (speed < 0) speed = 0;                    // no reversing

    x += cos(angle * DEG2RAD) * speed * dt;
    y += sin(angle * DEG2RAD) * speed * dt;

    fitness += speed * dt;                        // reward forward motion
}
```

**Death conditions:**
- Sensor value < `0.05f` → wall collision
- All sensors == `1.0f` → off track (no walls detected)
- Fitness gain < `350.0f` in last 3 seconds → stuck / spinning

**Finish line detection** uses segment intersection against the car's movement vector:

```cpp
Vector2 cross = getIntersection(finishLine, oldPos,
    {x + cos(angle * DEG2RAD) * 25.0f,
     y + sin(angle * DEG2RAD) * 25.0f});

if (cross.x != -1 && cross.y != -1) {
    fitness += 10000;
    foundPath = true;
}
```

---

### Genetic Algorithm — `GeneticAlgorithm.h / .cpp`

Manages a population of 100 cars. Runs one full evolution cycle when all cars are dead.

**One generation cycle:**

```
1. Sort population by fitness (highest first)
2. Extract elite weights (top N brains)
3. Breed 80 children via crossover
4. Mutate children
5. Reset all cars to spawn
```

**Crossover** — single-point cut on the flat weight vector:

```cpp
Car GeneticAlgo::createChild() {
    int rnd1 = rand() % eliteCount;
    int rnd2 = rand() % eliteCount;
    int cutPoint = rand() % 50;

    vector<float> childWeights;
    for (int i = 0; i < 50; i++)
        childWeights.push_back(i < cutPoint
            ? eliteWeights[rnd1][i]
            : eliteWeights[rnd2][i]);

    // mutate
    for (auto& w : childWeights) {
        float roll = (rand() % 101) / 100.0f;
        if (roll < mutationRate) {
            float nudge = -mutationStrength +
                (rand() / (float)RAND_MAX) * (2 * mutationStrength);
            w += nudge;
        }
    }

    Car child;
    child.brain.setWeights(childWeights);
    return child;
}
```

**Dynamic mutation** — mutation rate and strength increase when the population stagnates:

```cpp
if (population[0].fitness > bestFitnessEver) {
    bestFitnessEver = population[0].fitness;
    stuckGenerations = 0;
} else {
    stuckGenerations++;
}

mutationRate     = min(0.5f, 0.2f + stuckGenerations * 0.02f);
mutationStrength = min(0.9f, 0.5f + stuckGenerations * 0.05f);
```

This prevents the population from converging prematurely at local optima.

---

### Best Car — `BestCar.h / .cpp`

Inherits from `Car`. When a car first crosses the finish line, its brain is copied into a `BestCar` instance which then drives the track in demo mode — no death conditions, loops back to spawn on finish.

```cpp
void BestCar::Update(...) {
    // forward pass + movement only
    // no fitness tracking, no death checks
    // resets to spawn when finish line crossed
}
```

---

## Hyperparameters

| Parameter | Value | Effect |
|---|---|---|
| Population size | 100 | Number of cars per generation |
| Elite count | 10 | Top survivors used as parents |
| Mutation rate | 0.2 (dynamic) | Probability of mutating each weight |
| Mutation strength | 0.5 (dynamic) | Max nudge magnitude per mutation |
| Sensor length | 350px | Raycast range |
| Acceleration | 700 | Speed build-up rate |
| Retardation | -2.5 | Drag coefficient |
| Fitness threshold | 350 / 3sec | Min progress to stay alive |

---

## Fitness Function

```
fitness += speed * dt                    // reward forward motion
         + Σ(sensorValues[i]) * 0.1f   // reward staying centered on track
         + 10000                         // bonus for crossing finish line
```

Cars that drive fast and stay away from walls accumulate fitness faster. The 10000 bonus creates strong selection pressure toward actually completing the track.

---

## Track System

Tracks are drawn in-app by the user (Shift+Click to place points). The path is stored as a `vector<Vector2>` and converted to a `vector<Wall>` (line segments) for sensor intersection math.

The finish line is marked with Shift+F+Click on an existing path point. It is excluded from the wall vector during training so sensors don't treat it as an obstacle.

```cpp
// wall building — skip finish line segment
for (size_t i = 0; i + 1 < path.size(); i++) {
    Wall w = {path[i], path[i + 1]};
    if (w.a.x == finishLine.a.x && w.a.y == finishLine.a.y &&
        w.b.x == finishLine.b.x && w.b.y == finishLine.b.y)
        continue;
    walls.push_back(w);
}
```

Tracks can be copied (prints coordinates to console) and re-input for reproducible runs.

---

## Controls

| Input | Action |
|---|---|
| Shift + Click | Add path point |
| Shift + F + Click | Mark finish line |
| Right Click | Clear track and reset |
| Set Track button | Lock track and start simulation |
| Copy Track button | Print path coordinates to console |
| Input Track button | Load track from console coordinates |

---

## Dependencies

- [raylib](https://www.raylib.com/) — graphics, input, window management
- C++17 or later
- Standard library only (`<vector>`, `<cmath>`, `<algorithm>`, `<cstdlib>`)

---

## Build

```bash
g++ -std=c++17 src/*.cpp -o simulation -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

Or use your existing CMake / Makefile setup.

---

## Key Design Decisions

**Genetic algorithm over backpropagation** — with no labeled training data and a non-differentiable fitness signal (distance driven), gradient descent isn't applicable. The GA treats the weight space as a black-box optimization problem — no gradient required.

**Flat weight serialization** — representing the entire brain as a single `vector<float>` decouples the GA from the network's internal topology. Crossover and mutation operate on the flat representation; the network structure is irrelevant to the optimizer.

**Fitness function design** — rewarding raw speed caused cars to exploit straight sections and stall at corners. Adding a sensor-sum term `Σ(sensorValues) * 0.1f` penalizes wall-hugging and produces smoother cornering behavior implicitly.

**Dynamic mutation** — fixed hyperparameters cause premature convergence when the elite population homogenizes. Scaling mutation rate and strength with `stuckGenerations` maintains exploration pressure without manual retuning between runs.

**Finish line exclusion from wall vector** — including the finish line as a wall segment caused sensors to treat it as an obstacle, producing avoidance behavior near the goal. Excluding it at wall-build time eliminates the conflict without modifying sensor logic.