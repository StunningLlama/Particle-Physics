#include "stdafx.h"
#include "Bond.h"

Bond::Bond(Particle* A, Particle* B, int type, float len) {
	particleA = A;
	particleB = B;
	bondtype = type;
	length = len;
}