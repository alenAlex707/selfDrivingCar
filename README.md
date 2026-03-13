# Self-Driving Car Simulation using Neural Networks (Raylib + C++)

## Overview

This project is a **2D car simulation built with Raylib in C++** where a car drives along a **user-drawn path**. The long-term goal is to let the car **automatically figure out how to reach the finish line** using a simple neural network.

Instead of hardcoding movement, the car will eventually **learn to drive** on its own using sensor data.

This project is mainly about learning:

* Game physics
* Real-time input handling
* Path drawing systems
* Object-oriented programming in C++
* Neural networks
* Evolutionary and genetic algorithms

---

## Car Physics

The car simulation includes:

* Acceleration and reverse
* Rotation / steering
* Friction and drag
* Frame-rate independent movement using delta time

The car moves in the direction it is facing using trigonometry for velocity calculation, with frame-rate independent movement via delta time.

---

## Custom Path Drawing

The user draws a road by clicking on the screen. Each click records a point, which gets added to a list:

```cpp
vector<Vector2> path;
```

The points are then connected and drawn as lines to visualize the road.

---

## Neural Network (Planned)

The neural network will act as the **brain of the car** — it takes in what the car "sees" and decides how to steer and accelerate.

### Inputs — What the car sees

The car will have a few ray-based sensors pointing in different directions. Each one measures how far away the road edge is.

### Outputs — What the car does

The network will output two values:

```
Steering  →  turn left or right
Throttle  →  speed up or slow down
```

---

## Training (Planned)

The plan is to use a **genetic algorithm** — no manually labeled training data required.

1. Spawn multiple cars, each with a randomly initialized neural network
2. Let them attempt to drive the path
3. Keep the ones that travel the farthest
4. Mutate their networks slightly
5. Repeat over many generations

Over time, the cars should converge on a working driving strategy.

---

## Dependencies

* C++
* Raylib
* Standard C++ Libraries

---

## Project Status

**Complete:**
- Car movement physics
- Rotation and acceleration
- Path drawing system
- Car class structure
- Sensor system

**In progress:**
- Neural network integration
- AI training system

---

## Learning Goals

* Game physics simulation
* Real-time graphics programming
* Object-oriented design
* Neural network fundamentals
* Evolutionary algorithms
