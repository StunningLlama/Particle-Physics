#include "stdafx.h"
#include "Particle.h"

Particle::Particle() {

}

Particle::Particle(float ix, float iy, float ivx, float ivy, int iid, int elemi, float massi) {
	x = ix;
	y = iy;
	vx = ivx;
	vy = ivy;
	id = iid;
	material = elemi;
	mass = massi;
	//forces = new std::vector<Force>;
	//forces->reserve(20);
}
