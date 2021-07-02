#pragma once

class Simulation;

class Save {
public:
	void saveToFile(Simulation* sim);
	void loadFromFile(Simulation* sim);
};