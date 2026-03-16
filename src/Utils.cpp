#include "Utils.h"
#include <math.h>

float getDistance(Vector2 a, Vector2 b)
{
	return sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2));
}

Vector2 getIntersection(Wall wall, Vector2 sensorStart, Vector2 sensorEnd)
{
	Vector2 d1 = {(wall.b.x - wall.a.x), (wall.b.y - wall.a.y)};
	Vector2 d2 = {(sensorEnd.x - sensorStart.x), (sensorEnd.y - sensorStart.y)};

	float t, s;
	t = ((sensorStart.x - wall.a.x) * d2.y - (sensorStart.y - wall.a.y) * d2.x) / ((d1.x * d2.y) - (d1.y * d2.x));
	s = ((sensorStart.x - wall.a.x) * d1.y - (sensorStart.y - wall.a.y) * d1.x) / ((d1.x * d2.y) - (d1.y * d2.x));

	Vector2 intersection;

	if (t < 1 && t > 0 && s < 1 && s > 0)
	{
		intersection.x = wall.a.x + t * d1.x;
		intersection.y = wall.a.y + t * d1.y;
		return intersection;
	}

	return {-1, -1};
}

vector<float> normalize(vector<float> arr, float magnitude)
{
	vector<float> result{};
	for (int i = 0; i < arr.size(); i++)
	{
		result.push_back(arr[i] / magnitude);
	}
	return result;
}

bool buttonClicked(Rectangle rec)
{
	Vector2 mouse = GetMousePosition();
	if (CheckCollisionPointRec(mouse, rec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		return true;
	}
	return false;
}

bool buttonHover(Rectangle rec)
{
	Vector2 mouse = GetMousePosition();
	if (CheckCollisionPointRec(mouse, rec))
	{
		return true;
	}
	return false;
}