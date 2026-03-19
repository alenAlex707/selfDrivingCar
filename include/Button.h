#pragma once

#include "raylib.h"
#include <string>

using namespace std;

class Button
{
private:
  Rectangle bounds;
  string text;
  int fontSize;

public:
  Button(Rectangle btn, string label, int fontSize = 20);

  bool buttonHover();
  bool buttonClicked();

  void draw();
};