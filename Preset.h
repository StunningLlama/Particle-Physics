#pragma once

class Preset {
public:
	void startToilet();
	void genToilet();
	bool drawing = false;
	int frame = 0;

private:
	float cx = 200.0f;
	float cy = 80.0f;
	float innerradius = 30.0f;
	float outerradius = 50.0f;
	float thetai = 150.0f;
	float thetaf = 30.0f;
	float dist = 50.0f;

	float toRad(float deg);
};