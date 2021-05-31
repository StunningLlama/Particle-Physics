#pragma once
#include "Particle.h"
#include "Preset.h"
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <random>


#define sim_interactiondistancemax 7.5f
#define sim_interactiondistmaxsq sim_interactiondistancemax*sim_interactiondistancemax
#define sim_width 300.0f
#define sim_height 300.0f
#define sim_gridcolumns (int)(sim_width/sim_interactiondistancemax)
#define sim_gridrows (int)(sim_height/sim_interactiondistancemax)
#define sim_interactionresolution 15
#define sim_timestep 0.1f
#define sim_diffstrength_vel_para 0.4f
#define sim_diffstrength_vel_perp 0.4f
#define sim_buoyancy 0.4f
#define sim_mousestrength 20.0f

#define sim_materials 3

#define sim_type_water 0
#define sim_type_air 1
#define sim_type_barrier 2

#define sim_interactions 4

#define sim_interaction_none -1
#define sim_interaction_water_water 0
#define sim_interaction_air_air 1
#define sim_interaction_water_air 2
#define sim_interaction_water_barrier 3

#define sim_forcestrength_water 8.0f
#define sim_forcestrength_air 8.0f

class Simulation {
public:
	Simulation();

	void update();
	void applyPhys(int start, int end, int ind);
	void addRandomParticle();
	void addParticle(float x, float y, float vx, float vy, int type);
	void deleteParticle(int id);
	void clear();
	void Simulation::test(int i);
	float randFloat(float max);
	void getInput();
	void saveToFile(std::string name);
	void loadFromFile(std::string name);
	void drawLine(float xi, float yi, float ux, float uy, int ox, int oy, int tli2, float chdiameter, float mousedist, float critdistance, bool replace, int material, float exlen2);
	void fill();

	void waterWaterParams();
	void airAirParams();
	void waterAirParams();
	void waterBarrierParams();

	int particleid;
	float forcetable[sim_interactions][sim_interactionresolution + 1];
	float diffusiontable[sim_interactions][sim_interactionresolution + 1];
	float interactiondists[sim_interactions];
	float interactiondistssq[sim_interactions];
	int interactiontable[sim_materials][sim_materials];
	float mousetable[sim_interactionresolution + 1];
	float xbound = sim_gridcolumns*sim_interactiondistancemax;
	float ybound = sim_gridrows*sim_interactiondistancemax;
	float xsimarea = (sim_gridcolumns-2)*sim_interactiondistancemax;
	float ysimarea = (sim_gridrows-2)*sim_interactiondistancemax;
	float mousex_i = 0.0f, mousey_i = 0.0f, mousexpv = 0.0f, mouseypv = 0.0f, mousexpp = 0.0f, mouseypp = 0.0f, mousexppp = 0.0f, mouseyppp = 0.0f;
	bool mousedowntmp = false;
	float br_totlen = 0.0f;
	float br_extralength_2 = 0.0f;
	float density = 1.0f;
	unsigned char colortable[sim_materials][3] = { {0, 127, 255}, {127, 180, 196}, {127, 63, 0} };
	float masstable[sim_materials] = { 1.0f, 0.2f, 1.0f };
	float characteristicradius[sim_materials] = { 7.5f*0.5f, 7.5f*0.4f,  7.5f*0.3f };
	float drawradius[sim_materials] = { 7.5f, 7.5f, 7.5f };
	float tolerance[sim_materials] = { 1.4f, 1.4f, 1.32f };
	float jiggle[sim_materials][sim_materials] = {
			  /* W      A      B*/
		/*W*/ { 16.0f, 16.0f, 1.0f },
		/*A*/ { 16.0f, 8.0f, 0.0f },
		/*B*/ { 1.0f, 0.0f, 0.0f }};
	float gravity[sim_materials] = { 0.98f, 0.0f, 0.0f };

	Particle* grid[sim_gridcolumns][sim_gridrows];
	std::vector<Particle*> particles;
	//Particle particles[10000];
	std::thread *threadarray;
	std::vector<float> g_coords;
	std::vector<float> g_uvs;
	std::vector<float> g_pressures;
	std::vector<float> g_dbglines;
	std::vector<unsigned char> g_colors;

	Preset preset;

	//std::normal_distribution<float> normalDistr;
	//std::mt19937 generator;

	int threads;
	std::mutex *mtx;
	std::condition_variable *cv;
	bool *ready;
};
