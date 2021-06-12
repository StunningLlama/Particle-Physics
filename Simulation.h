#pragma once
#include "Particle.h"
#include "Preset.h"
#include "Rigidbody.h"
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <random>

#define BIG_CHUNGUS_SIZE

#define sim_interactiondistancemax 7.5f
#define sim_interactiondistmaxsq sim_interactiondistancemax*sim_interactiondistancemax

#ifdef BIG_CHUNGUS_SIZE
#define sim_width 800.0f
#define sim_height 500.0f
#else
#define sim_width 300.0f
#define sim_height 300.0f
#endif

#define sim_gridcolumns (int)(sim_width/sim_interactiondistancemax)
#define sim_gridrows (int)(sim_height/sim_interactiondistancemax)
#define sim_interactionresolution 15
#define sim_timestep 0.1f
#define sim_diffstrength_vel_para 0.4f
#define sim_diffstrength_vel_perp 0.4f
#define sim_buoyancy 0.4f
#define sim_mousestrength 20.0f

#define sim_materials 4

#define sim_type_water 0
#define sim_type_air 1
#define sim_type_barrier 2
#define sim_type_stone 3

#define sim_interactions 6

#define sim_interaction_none -1
#define sim_interaction_water_water 0
#define sim_interaction_air_air 1
#define sim_interaction_water_air 2
#define sim_interaction_water_barrier 3
#define sim_interaction_air_barrier 4
#define sim_interaction_stone_barrier 5

#define sim_boundary_walls 0
#define sim_boundary_windtunnel 1
#define sim_boundary_atmosphere 2

#define rngLUTsize 10000

#define pressureXpoints sim_gridcolumns*4
#define pressureYpoints sim_gridrows*4

#define EPSILON 0.0001f

class Simulation {
public:
	Simulation();

	void update();
	void applyPhys(int start, int end, int ind);
	void tryAddParticle(float fx, float fy, float critdistance, bool replace, int material);
	void addRandomParticle();
	void appBC();
	void addParticle(float x, float y, float vx, float vy, int type);
	void deleteParticle(int id);
	void addParticleToRigidBody(Particle* p, int rbid);
	void clear();
	void Simulation::test(int i);
	float randFloat(float max);
	float randFloatNormal();
	void getInput();
	void saveToFile(std::string name);
	void loadFromFile(std::string name);
	void drawLine(float xi, float yi, float ux, float uy, int ox, int oy, int tli2, float chdiameter, float mousedist, float critdistance, bool replace, int material, float exlen2, int objid);
	void fill();

	void waterWaterParams();
	void airAirParams();
	void waterAirParams();
	void waterBarrierParams();
	void airBarrierParams();
	void stoneBarrierParams();

	int particleid;
	float forcetable[sim_interactions][sim_interactionresolution + 1];
	float potentialtable[sim_interactions][sim_interactionresolution + 1];
	float diffusiontable[sim_interactions][sim_interactionresolution + 1];
	float interactiondists[sim_interactions];
	float interactiondistssq[sim_interactions];
	int interactiontable[sim_materials][sim_materials] = {
					/* W								 A								B								S*/
		/*W*/	{ sim_interaction_water_water,		sim_interaction_water_air,		sim_interaction_water_barrier,	sim_interaction_water_barrier },
		/*A*/	{ sim_interaction_water_air,		sim_interaction_air_air,		sim_interaction_air_barrier,	sim_interaction_air_barrier },
		/*B*/	{ sim_interaction_water_barrier,	sim_interaction_air_barrier,	sim_interaction_none,			sim_interaction_stone_barrier },
		/*S*/	{ sim_interaction_water_barrier,	sim_interaction_air_barrier,	sim_interaction_stone_barrier,	sim_interaction_stone_barrier } };
	float mousetable[sim_interactionresolution + 1];
	float xbound = sim_gridcolumns*sim_interactiondistancemax;
	float ybound = sim_gridrows*sim_interactiondistancemax;
	float xsimarea = (sim_gridcolumns-2)*sim_interactiondistancemax;
	float ysimarea = (sim_gridrows-2)*sim_interactiondistancemax;
	int boundarytype = sim_boundary_walls;
	float mousex_i = 0.0f, mousey_i = 0.0f, mousexpv = 0.0f, mouseypv = 0.0f, mousexpp = 0.0f, mouseypp = 0.0f, mousexppp = 0.0f, mouseyppp = 0.0f;
	bool mousedowntmp = false;
	float br_totlen = 0.0f;
	float br_extralength_2 = 0.0f;
	float density = 1.0f;
	float stime = 0.0f;
	float flowvelocity = 6.5f;
	float overallpressure = 0.0f;
	float avgoverallpressure = 0.0f;
	float maxlifespan = 1.0;
	float localpressure[pressureYpoints][pressureXpoints] = { 1.0 };
	float localpressureavg[pressureYpoints][pressureXpoints] = { 1.0 };
	float lpparticlecount[pressureXpoints][pressureYpoints] = { 1 };
			
	unsigned char colortable[sim_materials][3] =	{ {0, 127, 255},	{127, 180, 196},	{127, 63, 0},	{127, 127, 127}		};
	float masstable[sim_materials] =				{ 1.0f,				0.2f,				1.0f,			1.0f				};
	float characteristicradius[sim_materials] =		{ 7.5f*0.5f,		7.5f*0.4f,			7.5f*0.3f,		7.5f * 0.3f			};
	float drawradius[sim_materials] =				{ 7.5f,				7.5f,				7.5f,			7.5f				};
	float tolerance[sim_materials] =				{ 1.4f,				1.4f,				1.32f,			1.32f				};
	float gravity[sim_materials] =					{ 0.98f,			0.0f,				0.0f,			0.98f				};
	float jiggle[sim_materials][sim_materials] = {
				/* W		  A		 B			S*/
		/*W*/	{ 16.0f,	16.0f,	1.0f,	1.0f },
		/*A*/	{ 16.0f,	8.0f,	0.0f,	0.0f },
		/*B*/	{ 1.0f,		0.0f,	0.0f,	0.0f },
		/*S*/	{ 1.0f,		0.0f,	0.0f,	0.0f}};

	Particle* grid[sim_gridcolumns][sim_gridrows];
	std::vector<Particle*> particles;
	std::vector<Rigidbody*> objects;
	int objectid = 0;
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
	std::default_random_engine generator;
	std::normal_distribution<float> distribution;

	float rngLUT[rngLUTsize];
	int rngLUTindex = 0;

	int threads;
	std::mutex *mtx;
	std::condition_variable *cv;
	bool *ready;
};
