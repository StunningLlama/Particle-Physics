#include "stdafx.h"
#include "Bond.h"

Bond::Bond(Particle* A, Particle* B, int bondtype, float length, int id) {
	particleA = A;
	particleB = B;
	this->bondtype = bondtype;
	this->length = length;
	this->id = id;
}