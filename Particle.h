#pragma once

/*
struct Force {
	int id;
	float fx;
	float fy;
};
*/

class Particle {
public:
	Particle();
	Particle(float ix, float iy, float ivx, float ivy, int iid, int elemi, float massi);

	float x;
	float y;
	float vx = 0.0f;
	float vy = 0.0f;
	float dvx = 0.0f;
	float dvy = 0.0f;
	float mass = 1.0f;
	float pressure = 0.0f;
	float avgpressure = 0.0f;

	int gridx = 0;
	int gridy = 0;
	int id;
	int material;

	Particle *prev = nullptr;
	Particle *next = nullptr;
	//std::vector<Force> *forces;
};