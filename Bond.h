#pragma once
#include "Particle.h"
class Particle;

class Bond {
public:
	Bond(Particle* A, Particle* B, int type, float len);
	Particle* particleA;
	Particle* particleB;
	int bondtype = 0;
	int bondindex = 0;
	float length = 0.0f;
	bool close = false;
	bool variablelength = false;

	/*EXPERIMENTAL*/
	float xforceonA = 0.0f;
	float yforceonA = 0.0f;
	float compression = 0.0f;
	float shear = 0.0f;
	int rigidbodyid = -1;
	int rigidbodyindex = -1;
};