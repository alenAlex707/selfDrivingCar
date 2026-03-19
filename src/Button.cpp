#include "Button.h"

using namespace std;

Button::Button(Rectangle btn, string label, int fontSize)
{
  bounds = btn;
  text = label;
  this->fontSize = fontSize;
}

bool Button::buttonHover()
{
  Vector2 mouse = GetMousePosition();
  return CheckCollisionPointRec(mouse, bounds);
}

bool Button::buttonClicked()
{
  return buttonHover() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void Button::draw()
{
  Color color = GRAY;

  if (buttonHover())
    color = DARKGRAY;

  if (buttonHover() && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    color = LIGHTGRAY;

  DrawRectangleRec(bounds, color);

  int textWidth = MeasureText(text.c_str(), fontSize);

  float textX = bounds.x + (bounds.width - textWidth) / 2;
  float textY = bounds.y + (bounds.height - fontSize) / 2;

  DrawText(text.c_str(), textX, textY, fontSize, BLACK);
}