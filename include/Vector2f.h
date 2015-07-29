#pragma once

class Vector2f
{

public:
	Vector2f(float x, float y);

	Vector2f operator +(Vector2f &v2);
	Vector2f operator -(Vector2f &v2);
	Vector2f operator *(Vector2f &v2);

	normalize();
private:
	float x;
	float y;
}