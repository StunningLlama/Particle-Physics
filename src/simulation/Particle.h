#pragma once
#include <vector>
#include "Bond.h"
/*
struct Force {
	int id;
	float fx;
	float fy;
};
*/
class Bond;

class Particle {
public:
	Particle(float x, float y, float vx, float vy, int id, int elem, float mass);

	float x;
	float y;
	float vx;
	float vy;
	float mass;
	int material;
	int id;
	float forcex = 0.0f;
	float forcey = 0.0f;
	float pressure = 0.0f;
	float avgpressure = 0.0f;
	int strokenumber = 0;
	bool fixed = false;

	/*Air only*/
	float lifetime = 0.0f;
	bool deletionimminent = false;

	/*Sand only*/
	float size = 1.0f;

	/*Stone only*/
	int rigidbodyid = -1;
	int rigidbodyindex = -1;

	/*EXPERIMENTAL*/
	float max = 0.0f;
	float may = 0.0f;
	float minquantityx = 0.0f;
	float minquantityy = 0.0f;
	bool traversed = false;

	std::vector<Bond*> bonds;
	int numberofbonds = 0;

	int gridx = 0;
	int gridy = 0;
	Particle *prev = nullptr;
	Particle *next = nullptr;
	//std::vector<Force> *forces;
};