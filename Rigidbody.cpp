#include "stdafx.h"
#include "Rigidbody.h"
#include "Particle.h"
#include "Simulation.h"
#include "Life.h"
#include <iostream>

Coord::Coord(float ix, float iy) {
	x = ix;
	y = iy;
}

void Rigidbody::initializeUnconstrainedBody() {
	float comX = 0.0f;
	float comY = 0.0f;
	this->mass = 0.0f;
	for (int i = 0; i < numberofparticles; i++) {
		Particle* p = particles[i];
		comX += p->x * p->mass;
		comY += p->y * p->mass;
		this->mass += p->mass;
	}
	comX /= this->mass;
	comY /= this->mass;

	initializeFixedAxisBody(comX, comY);
	theta = 0.0f;
	fixed = false;
	physicsenabled = true;
	modified = false;

	//std::cout << "Initialized " << numberofparticles << " Particles!" << std::endl;
}

void Rigidbody::initializeFixedAxisBody(float axisX, float axisY) {
	moment = 0.0f;
	for (int i = 0; i < numberofparticles; i++) {
		Particle* p = particles[i];
		float rx = p->x - axisX;
		float ry = p->y - axisY;
		coordinates[i].x = rx;
		coordinates[i].y = ry;
		moment += (rx * rx + ry * ry) * p->mass;
	}

	x = axisX;
	y = axisY;

	theta = 0.0f;
	fixed = true;
	physicsenabled = true;
	modified = false;
}

void Rigidbody::move() {
	if (!physicsenabled)
		return;

	float netforcex = 0.0f;
	float netforcey = 0.0f;
	float torque = 0.0f;
	for (int i = 0; i < numberofparticles; i++) {
		Particle* p = particles[i];
		netforcex += p->forcex;
		netforcey += p->forcey;
		float rx = p->x - x;
		float ry = p->y - y;
		torque += rx * p->forcey - ry * p->forcex;

		if (!brittle) {
			p->forcex = 0.0f;
			p->forcey = 0.0f;
		}
	}

	if (!fixed) {
		vx += netforcex / mass * sim_timestep;
		vy += netforcey / mass * sim_timestep;
		x += vx * sim_timestep;
		y += vy * sim_timestep;

		if (brittle) {
			ax = netforcex / mass;
			ay = netforcey / mass;
		}
	}

	if (numberofparticles > 1) {
		omega += torque / moment * sim_timestep;
		theta += omega * sim_timestep;
		if (brittle) {
			alpha = torque / moment;
		}
	}
	else {
		theta = 0.0f;
		omega = 0.0f;
		alpha = 0.0f;
	}


	for (int i = 0; i < numberofparticles; i++) {
		Particle* p = particles[i];
		p->x = this->x + cos(theta) * coordinates[i].x - sin(theta) * coordinates[i].y;
		p->y = this->y + sin(theta) * coordinates[i].x + cos(theta) * coordinates[i].y;
		p->vx = this->vx + omega*(-sin(theta) * coordinates[i].x - cos(theta) * coordinates[i].y);
		p->vy = this->vy + omega*(cos(theta) * coordinates[i].x - sin(theta) * coordinates[i].y);
	}

	if (brittle) {
		computeStresses();
	}
}


void Rigidbody::computeStresses() {
	if (numberofparticles <= 1)
		return;

	for (int i = 0; i < numberofparticles; i++) {
		Particle* p = particles[i];
		p->max = p->mass * (ax + alpha * (-sin(theta) * coordinates[i].x - cos(theta) * coordinates[i].y) - omega * omega * (cos(theta) * coordinates[i].x - sin(theta) * coordinates[i].y));
		p->may = p->mass * (ay + alpha * (cos(theta) * coordinates[i].x - sin(theta) * coordinates[i].y) - omega * omega * (sin(theta) * coordinates[i].x + cos(theta) * coordinates[i].y));
	}

	float stepsize = 0.2f;
	float errorX = 99999.0f;
	float errorY = 99999.0f;
	int maxiterations = 200;
	int j = 0;
	for (j = 0; j < maxiterations && errorX + errorY > 0.2f; j++) {
		for (int i = 0; i < numberofparticles; i++) {
			Particle* p = particles[i];
			p->minquantityx = p->forcex - p->max;
			p->minquantityy = p->forcey - p->may;
		}
		for (int i = 0; i < numberofbonds; i++) {
			Bond* b = internalbonds[i];
			b->particleA->minquantityx += b->xforceonA;
			b->particleA->minquantityy += b->yforceonA;
			b->particleB->minquantityx -= b->xforceonA;
			b->particleB->minquantityy -= b->yforceonA;
		}
		for (int i = 0; i < numberofparticles; i++) {
			Particle* p = particles[i];
			errorX += p->minquantityx * p->minquantityx;
			errorY += p->minquantityy * p->minquantityy;
		}
		errorX /= numberofbonds;
		errorY /= numberofbonds;
		for (int i = 0; i < numberofbonds; i++) {
			Bond* b = internalbonds[i];
			b->xforceonA -= b->particleA->minquantityx * stepsize;
			b->yforceonA -= b->particleA->minquantityy * stepsize;
			b->xforceonA += b->particleB->minquantityx * stepsize;
			b->yforceonA += b->particleB->minquantityy * stepsize;
		}

	}
	std::cout << "MSEx " << errorX << ", MSEy " << errorY << ", Iterations "  << j << std::endl;
	for (int i = 0; i < numberofparticles; i++) {
		Particle* p = particles[i];
		p->pressure = 0.0f;
		for (Bond* b : p->bonds) {
			p->pressure += sqrt(b->xforceonA * b->xforceonA + b->yforceonA * b->yforceonA);
		}
	}

	bool brokenbonds = false;
	for (int i = 0; i < numberofbonds; i++) {
		Bond* b = internalbonds[i];
		Particle* p = b->particleA;
		Particle* q = b->particleB;
		float dist = sqrt((q->x - p->x) * (q->x - p->x) + (q->y - p->y) * (q->y - p->y));
		float xr = (q->x - p->x) / dist;
		float yr = (q->y - p->y) / dist;
		b->compression = b->xforceonA * xr + b->yforceonA * yr;
		b->shear = -b->xforceonA * yr + b->yforceonA * xr;
		if (b->compression < -60.0f || fabs(b->shear) > 60.0f || b->compression > 500.0f) {
			instance->sim->deleteBond(p, q);
			brokenbonds = true;
			i--;
		}
	}

	if (brokenbonds) {

		for (int i = 0; i < numberofparticles; i++) {
			particles[i]->traversed = false;
		}

		floodFill(particles[0], -1);
		for (int i = 1; i < numberofparticles; i++) {
			if (!particles[i]->traversed) {
				//std::cout << " Fracture at " << i << std::endl;
				instance->sim->objects.push_back(new Rigidbody);
				instance->sim->objectid++;
				floodFill(particles[i], instance->sim->objectid-1);
				instance->sim->objects[instance->sim->objectid - 1]->initializeUnconstrainedBody();
				instance->sim->objects[instance->sim->objectid - 1]->brittle = true;
				i--; 
			}
		}

		this->initializeUnconstrainedBody();
	}
}

void Rigidbody::floodFill(Particle* p, int newRigidbodyId) {
	p->traversed = true;
	if (newRigidbodyId != -1) {
		instance->sim->removeParticleFromRigidBody(p);
		instance->sim->addParticleToRigidBody(p, newRigidbodyId);
		for (int i = 0; i < p->numberofbonds; i++) {
			if (p->bonds[i]->rigidbodyid != newRigidbodyId) {
				instance->sim->removeBondFromRigidBody(p->bonds[i]);
				instance->sim->addBondToRigidBody(p->bonds[i], newRigidbodyId);
			}
		}
	}
	//std::cout << p->id << ".";
	//system("PAUSE");
	for (int i = 0; i < p->numberofbonds; i++) {
		Particle* q = p->bonds[i]->particleA;
		if (q == p)
			q = p->bonds[i]->particleB;
		if (!q->traversed)
			floodFill(q, newRigidbodyId);
	}
}