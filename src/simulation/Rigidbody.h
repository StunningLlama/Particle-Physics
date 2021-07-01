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
	Rigidbody(int id, bool brittle);
	std::vector<Particle*> particles;
	std::vector<Coord> coordinates;

	int id;
	float mass = 0.0f;
	float moment = 0.0f;
	float x = 0.0f;
	float y = 0.0f;
	float theta = 0.0f;
	float vx = 0.0f;
	float vy = 0.0f;
	float omega = 0.0f;

	bool physicsenabled = false;
	bool fixed = false;
	bool modified = false;
	int numberofparticles = 0;

	void initializeUnconstrainedBody();
	void initializeFixedAxisBody(float axisX, float axisY);
	void move();

	/*EXPERIMENTAL, todo: make compatible with deletion*/
	std::vector<Bond*> internalbonds;
	int numberofbonds = 0;
	bool brittle = false;
	float ax = 0.0f;
	float ay = 0.0f;
	float alpha = 0.0f;
	void computeStresses();
	void floodFill(Particle* p, Rigidbody* newRigidbody);
};