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

//#define BIG_CHUNGUS_SIZE

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
#define sim_bondinteractionresolution 30
#define sim_timestep 0.1f
//#define sim_diffstrength_vel_para 0.4f
//#define sim_diffstrength_vel_perp 0.4f
#define sim_buoyancy 0.4f
#define sim_mousestrength 20.0f

#define sim_materials 9

#define sim_type_water 0
#define sim_type_air 1
#define sim_type_barrier 2
#define sim_type_stone 3
#define sim_type_sand 4
#define sim_type_elastic 5
#define sim_type_cloth 6
#define sim_type_plastic 7
#define sim_type_brittle 8

#define sim_interactions 9

#define sim_interaction_none -1
#define sim_interaction_water_water 0
#define sim_interaction_air_air 1
#define sim_interaction_water_air 2
#define sim_interaction_water_barrier 3
#define sim_interaction_air_barrier 4
#define sim_interaction_stone_barrier 5
#define sim_interaction_sand_sand 6
#define sim_interaction_solid_solid 7
#define sim_interaction_cloth_cloth 8

//shorthand names
#define si_nointr -1
#define si_wtrwtr 0
#define si_airair 1
#define si_wtrair 2
#define si_wtrbar 3
#define si_airbar 4
#define si_stnbar 5
#define si_sndsnd 6
#define si_solsol 7
#define si_cloclo 8

#define sim_bonds 5
#define sim_bond_none -1
#define sim_bond_cloth 0
#define sim_bond_elastic 1
#define sim_bond_plastic 2
#define sim_bond_brittle 3
#define sim_bond_friction 4


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
	void addBond(Particle* p, Particle* q, int bondtype, float length);
	void deleteBond(Particle* p, Particle* q);
	void attachToNearbyParticles(Particle* p, int bondtype, float radius, bool respectbrushstroke);
	void clear();
	void Simulation::test(int i);
	float randFloat(float max);
	float randFloatNormal();
	float clamp(float x, float min, float max);
	void getInput();
	void saveToFile(std::string name);
	void loadFromFile(std::string name);
	void drawLine(float xi, float yi, float ux, float uy, int ox, int oy, int tli2, float chdiameter, float mousedist, float critdistance, bool replace, int material, float exlen2, int objid, int bondtype);
	void fill();

	void setPhysicsParameters();

	int particleid;
	int bondid;

	float forcetable[sim_interactions][sim_interactionresolution + 1];
	float potentialtable[sim_interactions][sim_interactionresolution + 1];
	float diffusiontable[sim_interactions][sim_interactionresolution + 1];
	float interactiondists[sim_interactions];
	float interactiondistssq[sim_interactions];
	float frictioncoefficients[sim_interactions];
	float jiggle[sim_interactions];
	int interactiontable[sim_materials][sim_materials] = {
					/*Water		Air			Barrier		Stone		Sand		Elastic		Cloth		Plastic		Brittle*/
		/*Water*/	{si_wtrwtr,	si_wtrair,	si_wtrbar,	si_wtrbar,	si_sndsnd,	si_wtrbar,	si_wtrbar,	si_wtrbar,	si_wtrbar},
		/*Air*/		{si_wtrair,	si_airair,	si_airbar,	si_airbar,	si_sndsnd,	si_airbar,	si_airbar,	si_airbar,	si_airbar},
		/*Barrier*/	{si_wtrbar,	si_airbar,	si_nointr,	si_stnbar,	si_sndsnd,	si_solsol,	si_cloclo,	si_solsol,	si_solsol},
		/*Stone*/	{si_wtrbar,	si_airbar,	si_stnbar,	si_stnbar,	si_sndsnd,	si_solsol,	si_cloclo,	si_solsol,	si_solsol},
		/*Sand*/	{si_sndsnd,	si_sndsnd,	si_sndsnd,	si_sndsnd,	si_sndsnd,	si_solsol,	si_cloclo,	si_solsol,	si_solsol},
		/*Elastic*/	{si_wtrbar,	si_airbar,	si_solsol,	si_solsol,	si_solsol,	si_solsol,	si_cloclo,	si_solsol,	si_solsol},
		/*Cloth*/	{si_wtrbar,	si_airbar,	si_cloclo,	si_cloclo,	si_cloclo,	si_cloclo,	si_cloclo,	si_cloclo,	si_cloclo},
		/*Plastic*/	{si_wtrbar,	si_airbar,	si_solsol,	si_solsol,	si_solsol,	si_solsol,	si_cloclo,	si_solsol,	si_solsol},
		/*Brittle*/	{si_wtrbar,	si_airbar,	si_solsol,	si_solsol,	si_solsol,	si_solsol,	si_cloclo,	si_solsol,	si_solsol}
	};

	float bondforcetable[sim_bonds][sim_bondinteractionresolution + 1];
	float bondfrictiontable[sim_bonds];
	float bondinteractiondists[sim_bonds];
	float bondequilibriumlengths[sim_bonds];

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

	const char* materialnames[sim_materials] =		{ "Water",			"Air",				"Barrier",			"Stone",			"Sand",				"Rubber",			"Cloth",			"Plastic",			"Ceramic"			};
	unsigned char colortable[sim_materials][3] =	{ {0, 127, 255},	{127, 180, 196},	{127, 63, 0},		{127, 127, 127},	{196, 196, 127},	{191, 58, 17},		{235, 167, 84},		{164, 175, 222},	{192, 186, 219}		};
	float masstable[sim_materials] =				{ 1.0f,				0.2f,				1.0f,				1.0f,				1.0f,				0.8,				0.6,				0.7,				1.0					};
	float characteristicradius[sim_materials] =		{ 7.5f*0.5f,		7.5f*0.4f,			7.5f*0.3f,			7.5f * 0.3f,		7.5f * 0.5f,		7.5f * 0.4f,		7.5f * 0.4f,		7.5f * 0.4f,		7.5f * 0.4f,		};
	float drawradius[sim_materials] =				{ 7.5f,				7.5f,				7.5f,				7.5f,				7.5f,				7.5f,				7.5f,				7.5f,				7.5f,				};
	float tolerance[sim_materials] =				{ 1.4f,				1.4f,				1.32f,				1.32f,				1.32f,				1.32f,				1.32f,				1.32f,				1.32f				};
	float gravity[sim_materials] =					{ 0.98f,			0.0f,				0.0f,				0.98f,				0.98f,				0.98f,				0.98f,				0.98f,				0.98f,				};
	int defaultbonds[sim_materials] =				{ sim_bond_none,	sim_bond_none,		sim_bond_none,		sim_bond_none,		sim_bond_none,		sim_bond_elastic,	sim_bond_cloth,		sim_bond_plastic,	sim_bond_brittle	};

	Particle* grid[sim_gridcolumns][sim_gridrows];
	std::vector<Particle*> particles;
	std::vector<Rigidbody*> objects;
	std::vector<Bond*> bonds;
	int objectid = 0;
	//Particle particles[10000];
	std::thread *threadarray;
	std::vector<float> g_coords;
	std::vector<float> g_uvs;
	std::vector<float> g_pressures;
	std::vector<float> g_bonds;
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
