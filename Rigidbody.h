#pragma once
#include <vector>
#include "Particle.h"

struct Coord {
	Coord(float ix, float iy);
	float x;
	float y;
};

class Rigidbody {
public:
	std::vector<Particle*> particles;
	std::vector<Coord> coordinates;

	float mass = 0.0f;
	float moment = 0.0f;

	float x = 0.0f;
	float y = 0.0f;
	float theta = 0.0f;
	float vx = 0.0f;
	float vy = 0.0f;
	float omega = 0.0f;

	bool fixed = false;
	int numberofparticles = 0;

	void initializeUnconstrainedBody();
	void initializeFixedAxisBody(float axisX, float axisY);
	void move();
};