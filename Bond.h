#pragma once
#include "Particle.h"
class Particle;

class Bond {
public:
	Bond(Particle* p, Particle* q, int type, float len, int id);
	Particle* particleA;
	Particle* particleB;
	int bondtype = 0;
	int id;
	float length = 0.0f;
	bool close = false;
	bool variablelength = false;
	bool deletionimminent = false;

	/*EXPERIMENTAL*/
	float xforceonA = 0.0f;
	float yforceonA = 0.0f;
	float compression = 0.0f;
	float shear = 0.0f;
	int rigidbodyid = -1;
	int rigidbodyindex = -1;
};