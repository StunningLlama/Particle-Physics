#include "stdafx.h"
#include "Rigidbody.h"
#include "Particle.h"
#include "Simulation.h"

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
		p->forcex = 0.0f;
		p->forcey = 0.0f;
	}

	if (!fixed) {
		vx += netforcex / mass * sim_timestep;
		vy += netforcey / mass * sim_timestep;
		x += vx * sim_timestep;
		y += vy * sim_timestep;
	}

	omega += torque / moment * sim_timestep;
	theta += omega * sim_timestep;


	for (int i = 0; i < numberofparticles; i++) {
		Particle* p = particles[i];
		p->x = this->x + cos(theta) * coordinates[i].x - sin(theta) * coordinates[i].y;
		p->y = this->y + sin(theta) * coordinates[i].x + cos(theta) * coordinates[i].y;
		p->vx = this->vx + omega*(-sin(theta) * coordinates[i].x - cos(theta) * coordinates[i].y);
		p->vy = this->vy + omega*(cos(theta) * coordinates[i].x - sin(theta) * coordinates[i].y);
	}
}