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
	float forcex = 0.0f;
	float forcey = 0.0f;
	float mass = 1.0f;
	float pressure = 0.0f;
	float avgpressure = 0.0f;
	float lifetime = 0.0f;
	bool deletionimminent = false;
	int rigidbodyid = -1;
	int rigidbodyindex = -1;
	int gridx = 0;
	int gridy = 0;
	int id;
	int material;

	Particle *prev = nullptr;
	Particle *next = nullptr;
	//std::vector<Force> *forces;
};