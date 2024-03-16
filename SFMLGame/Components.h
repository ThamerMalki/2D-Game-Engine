#pragma once

#include"Vec2.h"
#include"SFML/Graphics.hpp"

class CTransform
{
public:
	Vec2 position;
	Vec2 velocity;
	float angle;

	CTransform(const Vec2& pos, const Vec2& vel, float a=0.0f)
		: position(pos)
		, velocity(vel)
		, angle(a){}
};

class CShape
{
public:
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness)
		: circle(radius,points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}
};

class CCollision
{
public:
	float radius = 0;

	CCollision(float r)
		: radius(r){}
};

class CScore
{
public:
	int score = 0;

	CScore(int s)
	: score(s){}
};

class CLifeSpan
{
public:
	int total = 0;
	int remaining = 0;

	CLifeSpan(int t)
	: total(t)
	, remaining(t)
	{}
};

class CInput
{
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;

	CInput(){}
};