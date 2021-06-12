#include "stdafx.h"
#include "Simulation.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include "Particle.h"
#include "Life.h"
#include "Input.h"
#include "Timer.h"
#include "Rigidbody.h"
#include <vector>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>

Simulation::Simulation() {
	srand((unsigned int)time(NULL));
	particleid = 0;
	for (int x = 0; x < sim_gridcolumns; x++) {
		for (int y = 0; y < sim_gridrows; y++) {
			grid[x][y] = nullptr;
		}
	}
	//for (int i = 0; i < 3000; i++) {
	//	this->addRandomParticle();
	//}

	for (int i = 0; i < sim_interactionresolution + 1; i++) {
		float r = ((float)i) / (float)sim_interactionresolution;
		mousetable[i] = 1.0f - r;
	}

	waterWaterParams();
	airAirParams();
	waterAirParams();
	waterBarrierParams();
	airBarrierParams();
	stoneBarrierParams();


	for (int i = 0; i < sim_interactions; i++) {
		interactiondistssq[i] = interactiondists[i] * interactiondists[i];
		for (int j = 0; j < sim_interactionresolution; j++) {
			for (int k = j; k < sim_interactionresolution; k++) {
				potentialtable[i][j] += 0.5f*(forcetable[i][k] + forcetable[i][k+1]);
			}
		}
		potentialtable[i][sim_interactionresolution] = 0;
	}

	//density = 1.0*sqrt(particleid / (sim_gridrows*sim_gridcolumns));
	density = 2.0f;//1 / 0.5f;
	threads = std::thread::hardware_concurrency();
	//threads = 6;

	threadarray = new std::thread[threads];
	mtx = new std::mutex[threads];
	cv = new std::condition_variable[threads];
	ready = new bool[threads];

	distribution = std::normal_distribution<float>(0, 1.0);

	for (int i = 0; i < rngLUTsize; i++) {
		rngLUT[i] = distribution(generator);
	}

	for (int x = 0; x < pressureXpoints; x++) {
		for (int y = 0; y < pressureYpoints; y++) {
			localpressure[y][x] = 0.0;
		}
	}

	int *cuts = new int[threads + 1];

	for (int i = 0; i < threads + 1; i++) {
		cuts[i] = 0;
	}
	for (int i = 0; i < threads; i++) {
		cuts[i + 1] = (i + 1)*sim_gridcolumns*sim_gridrows / threads;
		//cout << cuts[i + 1] << endl;
	}

	for (int i = 0; i < threads; i++) {
		threadarray[i] = std::thread(&Simulation::applyPhys, this, cuts[i], cuts[i + 1], i);
		ready[i] = false;
		//threadarray[i] = std::thread(&Simulation::test, this, 0);
	}
	//normalDistr = normal_distribution<float>(0.0f, sim_jiggle_strength);
	//generator = mt19937(123);
}

void Simulation::waterWaterParams() {
	float waterforce = 8.0f;

	interactiondists[sim_interaction_water_water] = 7.5f;

	for (int i = 0; i < sim_interactionresolution + 1; i++) {
		float r = ((float)i) / (float)sim_interactionresolution;
		diffusiontable[sim_interaction_water_water][i] = (1.0f - r)*1.0f;
	}

	float k = 3.0f;
	float attraction = -0.1f;
	forcetable[sim_interaction_water_water][0] = 7 * k;
	forcetable[sim_interaction_water_water][1] = 6 * k;
	forcetable[sim_interaction_water_water][2] = 5 * k;
	forcetable[sim_interaction_water_water][3] = 4 * k;
	forcetable[sim_interaction_water_water][4] = 3 * k;
	forcetable[sim_interaction_water_water][5] = 2 * k;
	forcetable[sim_interaction_water_water][6] = 1 * k;
	forcetable[sim_interaction_water_water][7] = 0 * k;
	forcetable[sim_interaction_water_water][8] = 0 * k;
	forcetable[sim_interaction_water_water][9] = 0 * k;

	for (int i = 10; i <= 15; i++) {
		float x = (i - 10.0f) / (15.0f - 10.0f);
		//x = 1.0 - x;
		float a = 1.0f;
		float c = (1 - exp(-a) - (1 - exp(-a))*log((1 - exp(-a)) / a) - a) / a;
		float f = (exp(-a*x) + (1 - exp(-a))*x - 1) / c; 
		forcetable[sim_interaction_water_water][i] = f*attraction;
	}
	/*forcetable[sim_interaction_water_water][7] = 0.0f;
	forcetable[sim_interaction_water_water][8] = -0.3f*attraction;
	forcetable[sim_interaction_water_water][9] = -0.3f*attraction;
	forcetable[sim_interaction_water_water][10] = -0.2f*attraction;
	forcetable[sim_interaction_water_water][11] = -0.1f*attraction;
	forcetable[sim_interaction_water_water][12] = -0.06f*attraction;
	forcetable[sim_interaction_water_water][13] = -0.04f*attraction;
	forcetable[sim_interaction_water_water][14] = -0.02f*attraction;
	forcetable[sim_interaction_water_water][15] = 0.0f;*/

	for (int i = 0; i < sim_interactionresolution + 1; i++) {
		forcetable[sim_interaction_water_water][i] *= waterforce;
	}
}

void Simulation::airAirParams() {
	float airforce = 8.0f;
	interactiondists[sim_interaction_air_air] = 7.5f;
	for (int i = 0; i < sim_interactionresolution + 1; i++) {
		float r = ((float)i) / (float)sim_interactionresolution;
		forcetable[sim_interaction_air_air][i] = (1.0f-r)* airforce;
		diffusiontable[sim_interaction_air_air][i] = 0.125f*(1.0f - r);
	}
}

void Simulation::airBarrierParams() {
	float airforce = 8.0f;
	interactiondists[sim_interaction_air_barrier] = 7.5f;
	for (int i = 0; i < sim_interactionresolution + 1; i++) {
		float r = ((float)i) / (float)sim_interactionresolution;
		forcetable[sim_interaction_air_barrier][i] = (1.0f - r) * airforce;
		diffusiontable[sim_interaction_air_barrier][i] = 0.2f*0.125f * (1.0f - r);
	}
}

void Simulation::waterAirParams() {
	float airforce = 8.0f;
	interactiondists[sim_interaction_water_air] = 7.5f;
	for (int i = 0; i < sim_interactionresolution + 1; i++) {
		float r = ((float)i) / (float)sim_interactionresolution;
		forcetable[sim_interaction_water_air][i] = (1.0f - r)* airforce *2.0f;
		diffusiontable[sim_interaction_water_air][i] = 0.125f*(1.0f - r);
	}
}


void Simulation::waterBarrierParams() {
	interactiondists[sim_interaction_water_barrier] = 7.5f;
	for (int i = 0; i < sim_interactionresolution + 1; i++) {
		float r = ((float)i) / (float)sim_interactionresolution;
		diffusiontable[sim_interaction_water_barrier][i] = 0.1f*(1.0f - r);
	}


	for (int i = 0; i < sim_interactionresolution + 1; i++) {
		forcetable[sim_interaction_water_barrier][i] = forcetable[sim_interaction_water_water][i];
	}
}

void Simulation::stoneBarrierParams() {
	interactiondists[sim_interaction_stone_barrier] = 7.5f;
	for (int i = 0; i < 10 + 1; i++) {
		float r = ((float)i) / (float)8;
		forcetable[sim_interaction_stone_barrier][i] = exp(4.0f*(1.0f - r) -1.0f)/4.0f * 100.0f;
		diffusiontable[sim_interaction_stone_barrier][i] = 100.0f*0.4f* exp(4.0f * (1.0f - r) - 1.0f) / 4.0f;
	}
	for (int i = 11; i < sim_interactionresolution + 1; i++) {
		forcetable[sim_interaction_stone_barrier][i] = 0;
		diffusiontable[sim_interaction_stone_barrier][i] = 0;
	}
}

void Simulation::applyPhys(int start, int end, int ind) {
	while (true) {

		std::unique_lock<std::mutex> lck(mtx[ind]);
		while (!ready[ind]) cv[ind].wait(lck);

		for (int i = start; i < end; i++)
		{
			int px = i / sim_gridrows;
			int py = i % sim_gridrows;
			Particle *p = grid[px][py];
			while (p != nullptr) {

				float bdistdiffusion = 8.0 * sim_interactiondistancemax;

				float bproximitydiffusion = 1.0;

				if (p->x < bdistdiffusion) {
					bproximitydiffusion = fmin(bproximitydiffusion, p->x / bdistdiffusion);
					if (p->x < 0)
						bproximitydiffusion = 0;
				}
				if (p->y < bdistdiffusion) {
					bproximitydiffusion = fmin(bproximitydiffusion, p->y / bdistdiffusion);
					if (p->y < 0)
						bproximitydiffusion = 0;
				}
				if (xbound - p->x < bdistdiffusion) {
					bproximitydiffusion = fmin(bproximitydiffusion, (xbound - p->x) / bdistdiffusion);
					if (xbound - p->x < 0)
						bproximitydiffusion = 0;
				}
				if (ybound - p->y < bdistdiffusion) {
					bproximitydiffusion = fmin(bproximitydiffusion, (ybound - p->y) / bdistdiffusion);
					if (ybound - p->y < 0)
						bproximitydiffusion = 0;
				}

				bproximitydiffusion = (1.0f - bproximitydiffusion);
				//bproximitydiffusion = sin(bproximitydiffusion * 3.1415926);
				if (boundarytype == sim_boundary_walls)
					bproximitydiffusion = 0;
				float soundabsorptioncoefficient = (5.0 * bproximitydiffusion + 1.0);
				for (int dx = -1; dx <= 1; dx++) {
					for (int dy = -1; dy <= 1; dy++) {
						int gx = p->gridx + dx;
						int gy = p->gridy + dy;
						if (gx < 0 || gy < 0 || gx >= sim_gridcolumns || gy >= sim_gridrows)
							continue;
						Particle *q = grid[gx][gy];
						while (q != nullptr) {
							if (p->id > q->id)
							{
								int interactionnum = interactiontable[p->material][q->material];
								if (interactionnum == sim_interaction_none || (interactionnum == sim_interaction_stone_barrier && p->rigidbodyid != -1 && q->rigidbodyid != -1 && p->rigidbodyid == q->rigidbodyid)) {
									q = q->next;
									continue;
								}
								float distsq = (q->x - p->x)*(q->x - p->x) + (q->y - p->y)*(q->y - p->y);
								if (distsq < interactiondistssq[interactionnum] && distsq > 0) {
									float interactionmaxdist = interactiondists[interactionnum];
									float dist = sqrt(distsq);
									float dn = (dist / interactionmaxdist)*sim_interactionresolution;
									int di = (int)dn;
									float df = dn - di;
									float force = forcetable[interactionnum][di] * (1.0f - df) + forcetable[interactionnum][di + 1] * df;
									float diffusion = diffusiontable[interactionnum][di] * (1.0f - df) + diffusiontable[interactionnum][di + 1] * df;

									force = force * p->lifetime * q->lifetime;
									diffusion = diffusion * p->lifetime * q->lifetime * soundabsorptioncoefficient;

									float xr = (q->x - p->x) / dist;
									float yr = (q->y - p->y) / dist;

									float deltavx = (p->vx - q->vx);
									float deltavy = (p->vy - q->vy);
									float fparallel = (deltavx*xr + deltavy*yr)*sim_diffstrength_vel_para*diffusion;
									if (interactionnum == sim_interaction_water_water)
										fparallel *= 4.0f;
									float fperp = (-deltavx * yr + deltavy * xr) * sim_diffstrength_vel_perp * diffusion + diffusion/soundabsorptioncoefficient* randFloatNormal()*jiggle[p->material][q->material];
									if (interactionnum == sim_interaction_stone_barrier)
										fperp += (-deltavx * yr + deltavy * xr) >= 0? diffusion*1.25f: -diffusion * 1.25f;
									float fx = force*xr + xr*fparallel - yr*fperp;
									float fy = force*yr + yr*fparallel + xr*fperp + sim_buoyancy*(p->mass-q->mass)*diffusion;
									p->forcex -= fx;
									p->forcey -= fy;
									q->forcex += fx;
									q->forcey += fy;

									/*Force f;
									f.id = q->id;
									f.fx = -fx;
									f.fy = -fy;
									p->forces->push_back(f);

									f.id = p->id;
									f.fx = fx;
									f.fy = fy;
									q->forces->push_back(f);*/

									p->pressure += force/dist;
									q->pressure += force/dist;
								}
							}
							q = q->next;
						}
					}
				}

				/*float randr = randFloat(1.0f);
				float randtheta = randFloat(2 * M_PI);
				p->dvx += randr*cos(randtheta);
				p->dvy += randr*sin(randtheta);*/
				int interactiontype = interactiontable[sim_type_barrier][p->material];
				float boundarydist = 1.0f;
				if (boundarytype == sim_boundary_windtunnel)
					boundarydist = 2.0 * sim_interactiondistancemax;
				else if (boundarytype == sim_boundary_walls)
					boundarydist = interactiondists[interactiontype];
				float dn = 1.0f;

				if (p->x < boundarydist) {
					dn = fmin(dn, (p->x / boundarydist));
					if (p->x < 0)
						dn = 0;
				}
				if (p->y < boundarydist) {
					dn = fmin(dn, (p->y / boundarydist));
					if (p->y < 0)
						dn = 0;
				}
				if (xbound - p->x < boundarydist) {
					dn = fmin(dn, ((xbound - p->x) / boundarydist));
					if (xbound - p->x < 0)
						dn = 0;
				}
				if (ybound - p->y < boundarydist) {
					dn = fmin(dn, ((ybound - p->y) / boundarydist));
					if (ybound - p->y < 0)
						dn = 0;
				}


				float boundaryforcex = 0.0;
				float boundaryforcey = 0.0;
				float disttoboundary = dn*boundarydist;

				if (boundarytype == sim_boundary_windtunnel)
				{
					float bp = 0.0f;
					if (p->x < boundarydist * 2) {
						boundaryforcex += 0.2f * (bp * (1.0f - dn) + 5.0f * (flowvelocity - p->vx));
					}
					if (p->y < boundarydist) {
						boundaryforcey += 0.2f * (bp * (1.0f - dn));
					}
					if (xbound - p->x < boundarydist) {
						boundaryforcex += -0.2f * (bp * (1.0f - dn)) + 0.2f * 5.0f * (flowvelocity - p->vx);
					}
					if (ybound - p->y < boundarydist) {
						boundaryforcey += -0.2f * (bp * (1.0f - dn));
					}
				}
				else if (boundarytype == sim_boundary_walls)
				{
					if (p->x < boundarydist) {
						float dn = (p->x / boundarydist) * sim_interactionresolution;
						if (p->x < 0)
							dn = 0;
						int di = (int)dn;
						float df = dn - di;
						float force = forcetable[interactiontype][di] * (1.0f - df) + forcetable[interactiontype][di + 1] * df;
						float diffusion = diffusiontable[interactiontype][di] * (1.0f - df) + diffusiontable[interactiontype][di + 1] * df;
						p->forcex += density * (force - p->vx * sim_diffstrength_vel_para * diffusion);
						p->forcey += -density * p->vy * sim_diffstrength_vel_perp * diffusion;
					}
					if (p->y < boundarydist) {
						float dn = (p->y / boundarydist) * sim_interactionresolution;
						if (p->y < 0)
							dn = 0;
						int di = (int)dn;
						float df = dn - di;
						float force = forcetable[interactiontype][di] * (1.0f - df) + forcetable[interactiontype][di + 1] * df;
						float diffusion = diffusiontable[interactiontype][di] * (1.0f - df) + diffusiontable[interactiontype][di + 1] * df;
						p->forcey += density * (force - p->vy * sim_diffstrength_vel_para * diffusion);
						p->forcex += -density * p->vx * sim_diffstrength_vel_perp * diffusion;
					}
					if (xbound - p->x < boundarydist) {
						float dn = ((xbound - p->x) / boundarydist) * sim_interactionresolution;
						if (xbound - p->x < 0)
							dn = 0;
						int di = (int)dn;
						float df = dn - di;
						float force = forcetable[interactiontype][di] * (1.0f - df) + forcetable[interactiontype][di + 1] * df;
						float diffusion = diffusiontable[interactiontype][di] * (1.0f - df) + diffusiontable[interactiontype][di + 1] * df;
						p->forcex += density * (-force - p->vx * sim_diffstrength_vel_para * diffusion);
						p->forcey += -density * p->vy * sim_diffstrength_vel_perp * diffusion;
					}
					if (ybound - p->y < boundarydist) {
						float dn = ((ybound - p->y) / boundarydist) * sim_interactionresolution;
						if (ybound - p->y < 0)
							dn = 0;
						int di = (int)dn;
						float df = dn - di;
						float force = forcetable[interactiontype][di] * (1.0f - df) + forcetable[interactiontype][di + 1] * df;
						float diffusion = diffusiontable[interactiontype][di] * (1.0f - df) + diffusiontable[interactiontype][di + 1] * df;
						p->forcey += density * (-force - p->vy * sim_diffstrength_vel_para * diffusion);
						p->forcex += -density * p->vx * sim_diffstrength_vel_perp * diffusion;
					}
				}

				p->forcex += boundaryforcex;
				p->forcey += boundaryforcey;
				if (disttoboundary > EPSILON)
					p->pressure += sqrt(boundaryforcex * boundaryforcex + boundaryforcey * boundaryforcey)/disttoboundary;


				//if (p->material == sim_type_air)
				//	p->dvy -= 0.2f;
				//else
				p->forcey -= gravity[p->material]*p->mass;

				p = p->next;
			}
		}

		ready[ind] = false;
		cv[ind].notify_all();
	}
}

void Simulation::test(int i) {
	//std::cout << "Meow" << i;
}

void Simulation::update() {
	instance->phys_1.start();

	if (!instance->input->paused || instance->input->nextframe) {
		for (int i = 0; i < threads; i++) {
			std::unique_lock<std::mutex> lck(mtx[i]);
			ready[i] = true;
			cv[i].notify_all();
		}

		for (int i = 0; i < threads; i++) {
			std::unique_lock<std::mutex> lck(mtx[i]);
			while (ready[i]) cv[i].wait(lck);
		}
	}


	instance->phys_1.end();

	this->getInput();

	instance->phys_2.start();
	if (!instance->input->paused || instance->input->nextframe) {
		if (boundarytype == sim_boundary_windtunnel)
			this->appBC();
		stime += sim_timestep;

		overallpressure = 0;
		int nump = 1;

		for (int x = 0; x < pressureXpoints; x++) {
			for (int y = 0; y < pressureYpoints; y++) {
				lpparticlecount[x][y] = 0;
			}
		}

		for (int i = 0; i < objectid; i++) {
			if (objects[i]->numberofparticles == 0 && objects[i]->physicsenabled) {
				delete objects[i];
				if (i < objectid - 1) {
					objects[i] = objects[objectid - 1];
					for (int j = 0; j < objects[i]->numberofparticles; j++) {
						objects[i]->particles[j]->rigidbodyid = i;
					}
				}
				objects.pop_back();
				objectid--;
				i--;
			} else if (objects[i]->physicsenabled){
				if (objects[i]->modified) {
					if (objects[i]->fixed)
						objects[i]->initializeFixedAxisBody(objects[i]->x, objects[i]->y);
					else
						objects[i]->initializeUnconstrainedBody();
				}
				objects[i]->move();
			}	
		}

		for (int i = 0; i < particleid; i++) {
			Particle *p = particles[i];
			if (p->material != sim_type_barrier && p->material != sim_type_stone) {
				p->vx += p->forcex/p->mass*sim_timestep;
				p->vy += p->forcey/p->mass*sim_timestep;
				p->x += p->vx*sim_timestep;
				p->y += p->vy*sim_timestep;
			}
			p->forcex = 0;
			p->forcey = 0;

			if (p->x < 0.0f)
				p->x = 0.0f;
			if (p->x >= xbound)
				p->x = xbound - EPSILON;
			if (p->y < 0.0f)
				p->y = 0.0f;
			if (p->y >= ybound)
				p->y = ybound - EPSILON;

			if (p->deletionimminent == true) {
				p->lifetime -= sim_timestep / maxlifespan;
			}
			else if (p->lifetime < 1.0f) {
				p->lifetime += sim_timestep / maxlifespan;
			}
			if (p->lifetime > 1.0f) {
				p->lifetime = 1.0f;
			}

			if (p->material == sim_type_air) {
				float boundarydist = 2.0 * sim_interactiondistancemax;
				if (p->x > boundarydist * 2 && p->y > boundarydist && xbound - p->x > boundarydist && ybound - p->y > boundarydist) {
					overallpressure += p->pressure;
					nump++;
				}
				int pnx = (int)(p->x / sim_width * pressureXpoints);
				int pny = (int)(p->y / sim_height * pressureYpoints);
				localpressure[pny][pnx] += p->pressure;
				lpparticlecount[pnx][pny] ++;
			}

			p->avgpressure = p->avgpressure * 0.95f + p->pressure * 0.05f;
			p->pressure = 0.0f;

			int nx = (int)(p->x / sim_interactiondistancemax);
			int ny = (int)(p->y / sim_interactiondistancemax);
			if (nx != p->gridx || ny != p->gridy) {
				if (p->prev != nullptr)
					p->prev->next = p->next;
				else
					grid[p->gridx][p->gridy] = p->next;

				if (p->next != nullptr)
					p->next->prev = p->prev;

				p->gridx = nx;
				p->gridy = ny;
				p->next = grid[p->gridx][p->gridy];
				grid[p->gridx][p->gridy] = p;
				if (p->next != nullptr)
					p->next->prev = p;
				p->prev = nullptr;
			}
		}
		overallpressure /= (float)nump;
		avgoverallpressure = avgoverallpressure * 0.98 + overallpressure * 0.02;


		for (int x = 0; x < pressureXpoints; x++) {
			for (int y = 0; y < pressureYpoints; y++) {
				if (lpparticlecount[x][y] == 0) {
					//localpressure[y][x] = 0;
					//Do nothing
				}
				else {
					localpressure[y][x] /= lpparticlecount[x][y];
					localpressure[y][x] -= avgoverallpressure;
					localpressure[y][x] = localpressure[y][x] * instance->gphx->pressurecontrast + 0.5f + instance->gphx->pressureoffset;
				}
				localpressureavg[y][x] = localpressureavg[y][x] * 0.98f + localpressure[y][x] * 0.02f;
			}
		}
	}
	instance->phys_2.end();

	if (instance->input->nextframe)
		instance->input->nextframe = false;
}

void Simulation::appBC() {

	float cp = 35.0;
	float wt_a = 1.0f;
	float wt_b = 0.4f;
	float wt_c = 2.0f;

	float spacing = 1.5f;
	float spacinga = spacing * 1.0f;
	float spacingb = spacing * 1.5f;
	float spacingc = spacing * 0.5f;

	float offset = 2.5f;
	for (float fy = 0.1; fy < ybound; fy += spacinga) {
		tryAddParticle(offset + randFloat(spacinga) - spacinga/2.0f, fy + randFloat(spacinga) - spacinga/2.0f, spacinga, false, sim_type_air);
	}
	for (float fy = 0.1; fy < ybound; fy += spacingb) {
		tryAddParticle(xbound - offset + randFloat(spacingb) - spacingb / 2.0f, fy + randFloat(spacingb) - spacingb / 2.0f, spacingb, false, sim_type_air);
	}
	for (float fx = 0.1; fx < xbound; fx += spacingc) {
		tryAddParticle(fx + randFloat(spacingc) - spacingc / 2.0f, offset + randFloat(spacingc) - spacingc / 2.0f, spacingc, false, sim_type_air);
	}
	for (float fx = 0.1; fx < xbound; fx += spacingc) {
		tryAddParticle(fx + randFloat(spacingc) - spacingc / 2.0f, ybound - offset + randFloat(spacingc) - spacingc / 2.0f, spacingc, false, sim_type_air);
	}


	for (int px = 0; px < sim_gridcolumns; px++)
	{
		for (int py = 0; py < sim_gridrows; py++)
		{
			Particle* p = grid[px][py];
			while (p != nullptr) {
				Particle* q = p;
				p = p->next;
				if (q->lifetime < 0)
				{
					deleteParticle(q->id);
				}
				else if (py >= sim_gridrows - 2 || py < 2) {
					if (q->pressure > cp * wt_c)
						q->deletionimminent = true;
				}
				else if (px >= sim_gridcolumns - 4)
				{
					if (q->pressure > cp * wt_b)
						q->deletionimminent = true;
				}
				else if (px < 4)
				{
					if (q->pressure > cp * wt_a)
						q->deletionimminent = true;
				}
			}
		}
	}

	/*for (int px = sim_gridcolumns - 2; px < sim_gridcolumns; px++)
	{
		for (int py = 0; py < sim_gridrows; py++)
		{
			Particle* p = grid[px][py];
			while (p != nullptr) {
				Particle* q = p;
				p = p->next;
				if (q->pressure > cp*0.6)
					deleteParticle(q->id);
			}
		}
	}
	for (int px = 0; px < 2; px++)
	{
		for (int py = 0; py < sim_gridrows; py++)
		{
			Particle* p = grid[px][py];
			while (p != nullptr) {
				Particle* q = p;
				p = p->next;
				if (q->pressure > cp)
					deleteParticle(q->id);
			}
		}
	}
	for (int px = 0; px < sim_gridcolumns; px++)
	{
		for (int py = sim_gridrows - 2; py < sim_gridrows; py++)
		{
			Particle* p = grid[px][py];
			while (p != nullptr) {
				Particle* q = p;
				p = p->next;
				if (q->pressure > cp)
					deleteParticle(q->id);
			}
		}
	}
	for (int px = 0; px < sim_gridcolumns; px++)
	{
		for (int py = 0; py < 2; py++)
		{
			Particle* p = grid[px][py];
			while (p != nullptr) {
				Particle* q = p;
				p = p->next;
				if (q->pressure > cp)
					deleteParticle(q->id);
			}
		}
	}*/
}

auto lastt = std::chrono::high_resolution_clock::now();

void Simulation::getInput() {
	std::chrono::high_resolution_clock::time_point thist = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(thist - lastt);
	lastt = thist;

	if (preset.drawing)
		preset.genAirfoil();

	float mousex_f = instance->input->coordX;
	float mousey_f = instance->input->coordY;

	int brushmode = instance->input->modebrush;


	if (brushmode == mode_brush_move || brushmode == mode_brush_delete) {

		if (instance->input->mouseDownL || instance->input->mouseDownR && brushmode == mode_brush_delete) {

			float vx = (mousex_f - mousex_i) / (float)time_span.count();
			float vy = (mousey_f - mousey_i) / (float)time_span.count();

			int nx = (int)(mousex_f / sim_interactiondistancemax);
			int ny = (int)(mousey_f / sim_interactiondistancemax);


			float mousedist = instance->input->brushsize*0.05f*xbound;
			if (brushmode == mode_brush_delete) {
				mousedist = ((float)instance->input->brushsize + 0.5f)*characteristicradius[instance->input->modematerial]*0.5f * instance->input->densitytable[instance->input->density - 1];
			}
			int gridcells = (int)(ceil(mousedist / sim_interactiondistancemax));

			for (int dx = -gridcells; dx <= gridcells; dx++) {
				for (int dy = -gridcells; dy <= gridcells; dy++) {
					int gx = nx + dx;
					int gy = ny + dy;
					if (gx < 0 || gy < 0 || gx >= sim_gridcolumns || gy >= sim_gridrows)
						continue;
					Particle *q = grid[gx][gy];
					while (q != nullptr) {
						float dist = sqrt((q->x - mousex_f)*(q->x - mousex_f) + (q->y - mousey_f)*(q->y - mousey_f));
						Particle *qtmp = q;
						q = q->next;
						if (dist < mousedist && dist > 0) {
							if (brushmode == mode_brush_move) {
								float dn = (dist / mousedist * sim_interactionresolution);
								int di = (int)dn;
								float df = dn - di;
								float force = sim_mousestrength*(mousetable[di] * (1.0f - df) + mousetable[di + 1] * df) / xbound;
								qtmp->forcex += vx*force*qtmp->mass;
								qtmp->forcey += vy*force*qtmp->mass;
							}
							else if (brushmode == mode_brush_delete) {
								deleteParticle(qtmp->id);
							}
						}
					}
				}
			}
		}
	}
	else if (brushmode == mode_brush_add || brushmode == mode_brush_replace) {

		bool drawrigidobject = (instance->input->modematerial == sim_type_stone);

		if (!mousedowntmp && instance->input->mouseDownL) {
			mousexpp = mousex_f;
			mouseypp = mousey_f;
			mousexppp = mousex_f;
			mouseyppp = mousey_f;
			mousexpv = 0;
			mouseypv = 0;
			br_totlen = 0;
			br_extralength_2 = 0;

			if (drawrigidobject) {
				objects.push_back(new Rigidbody);
				objectid++;
			}
			//out << "[ress";
		} else if (mousedowntmp && !instance->input->mouseDownL) {

			if (drawrigidobject) {
				objects[objectid - 1]->initializeUnconstrainedBody();
			}
		}

		float ds = sqrt((mousex_f - mousex_i)*(mousex_f - mousex_i) + (mousey_f - mousey_i)*(mousey_f - mousey_i));

		float chradius = characteristicradius[instance->input->modematerial]/2.0f * instance->input->densitytable[instance->input->density - 1];
		float chdiameter = chradius*2.0f;

		float mousedist = ((float)instance->input->brushsize + 0.5f)*chdiameter;

		int ox = 2*(int)(ceil(mousedist / chdiameter));
		int oy = (int)(ceil(mousedist / (chdiameter*sqrt(3)/2)));

		br_totlen += 4.0f*ds / chdiameter;

		int ipart = (int)br_totlen;
		float fpart = br_totlen - ipart;


		if (ipart > 0) {


			//std::cout << "hey" << std::endl;
			float forcex = (mousex_f - mousexpp)*0.5f - mousexpv*1.414f;
			float forcey = (mousey_f - mouseypp)*0.5f - mouseypv*1.414f;

			float ts = 0.6f;

			mousexpv += forcex*ts;
			mouseypv += forcey*ts;

			mousexpp += mousexpv*ts;
			mouseypp += mouseypv*ts;

			float dsp2 = sqrt((mousexpp - mousexppp)*(mousexpp - mousexppp) + (mouseypp - mouseyppp)*(mouseypp - mouseyppp));


			float ux = mousexpv;
			float uy = mouseypv;

			float dsp = sqrt(ux*ux + uy*uy);
			if (dsp > 0.0f) {
				ux /= dsp;
				uy /= dsp;
			}
			

			float tl2 = (dsp2) / chdiameter + br_extralength_2;
			int tli2 = (int)tl2;
			float tlf2 = tl2 - tli2;
			
			float critdistance = tolerance[instance->input->modematerial]*chradius;
			if (instance->input->mouseDownL && tli2>0 && dsp2>0) {
				if (drawrigidobject)
					drawLine(mousexppp, mouseyppp, ux, uy, ox, oy, tli2, chdiameter, mousedist, critdistance, brushmode == mode_brush_replace, instance->input->modematerial, br_extralength_2, objectid - 1);
				else
					drawLine(mousexppp, mouseyppp, ux, uy, ox, oy, tli2, chdiameter, mousedist, critdistance, brushmode == mode_brush_replace, instance->input->modematerial, br_extralength_2, -1);
			}


			mousexppp = mousexpp;
			mouseyppp = mouseypp;
			if (!mousedowntmp && instance->input->mouseDownL) {
				br_extralength_2 = 0;
			}
			else {
				br_extralength_2 = tlf2;
			}
		}
		if (!mousedowntmp && instance->input->mouseDownL) {
			br_totlen = 0;
		}
		else {
			br_totlen = fpart;
		}
		mousedowntmp = instance->input->mouseDownL;
	}


	mousex_i = mousex_f;
	mousey_i = mousey_f;


	if (instance->input->save) {
		std::string name;
		std::cout << "Please enter file name. ";
		std::cin >> name;
		saveToFile(name);
		instance->input->save = false;
	}

	if (instance->input->load) {
		std::string name;
		std::cout << "Please enter file name. ";
		std::cin >> name;
		loadFromFile(name);
		instance->input->load = false;
	}

	if (instance->input->clear) {
		this->clear();
		instance->input->clear = false;
	}

	if (instance->input->specialfunc) {
		fill();
		instance->input->specialfunc = false;
	}
}

void Simulation::fill() {
	int material = sim_type_air;
	float chradius = characteristicradius[material] / 2.0f;
	float chdiameter = chradius*2.0f;

	float mousedist = ybound / 2.0f;

	int ox = 2 * (int)(ceil(mousedist / chdiameter));
	int oy = (int)(ceil(mousedist / (chdiameter*sqrt(3) / 2)));

	float x1 = 0.0f;
	float x2 = xbound;
	float y1 = ybound / 2.0f;
	float y2 = ybound / 2.0f;

	float dsp2 = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));


	float ux = x2-x1;
	float uy = y2-y1;

	float dsp = sqrt(ux*ux + uy*uy);
	if (dsp > 0.0f) {
		ux /= dsp;
		uy /= dsp;
	}

	float tl2 = (dsp2) / chdiameter;
	int tli2 = (int)tl2;
	float tlf2 = tl2 - tli2;

	float critdistance = tolerance[sim_type_air] * chradius;

	drawLine(x1, y1, ux, uy, ox, oy, tli2, chdiameter, mousedist, critdistance, false, material, 0.0f, -1);
}

void Simulation::drawLine(float xi, float yi, float ux, float uy, int ox, int oy, int tli2, float chdiameter, float mousedist, float critdistance, bool replace, int material, float exlen2, int objid) {
	float costheta = 0.5;
	float sintheta = sqrt(3.0f) / 2.0f;

	float uxp = ux*costheta - uy*sintheta;
	float uyp = ux*sintheta + uy*costheta;

	for (int ex = 1 - ox; ex <= tli2 + ox; ex++) {
		for (int ey = -oy; ey <= oy; ey++) {
			float cx = ex*chdiameter;
			float cy = ey*chdiameter;
			float fx = xi + (cx - exlen2*chdiameter)*ux + cy * uxp;
			float fy = yi + (cx - exlen2*chdiameter)*uy + cy * uyp;
			bool withinradius = false;
			float cxp = cx + 0.5f*cy;
			float cyp = cy*sqrt(3.0f) / 2.0f;
			if (cxp < 0) {
				withinradius = sqrt(cxp*cxp + cyp*cyp) < mousedist;
			}
			else if (cxp > (tli2*chdiameter)) {
				withinradius = sqrt((cxp - tli2*chdiameter)*(cxp - tli2*chdiameter) + cyp*cyp) < mousedist;
			}
			else {
				withinradius = cyp < mousedist && cyp > -mousedist;
			}
			if (withinradius) {
				if (fx > 0 && fy > 0 && fx < xbound && fy < ybound) {
					int gxi = (int)(fx / sim_interactiondistancemax);
					int gyi = (int)(fy / sim_interactiondistancemax);
					float canadd = true;
					for (int dx = -1; dx <= 1; dx++) {
						for (int dy = -1; dy <= 1; dy++) {
							int gx = gxi + dx;
							int gy = gyi + dy;
							if (gx < 0 || gy < 0 || gx >= sim_gridcolumns || gy >= sim_gridrows)
								continue;
							Particle *q = grid[gx][gy];
							while (q != nullptr) {
								float distsq = sqrt((q->x - fx)*(q->x - fx) + (q->y - fy)*(q->y - fy));
								Particle *qtmp = q;
								q = q->next;
								if (distsq < critdistance) {
									if (replace || qtmp->material == sim_type_air) {
										deleteParticle(qtmp->id);
									}
									else {
										canadd = false;
									}
								}
							}
						}
					}
					if (canadd) {
						addParticle(fx, fy, 0.0f, 0.0f, material);
						if (objid != -1) {
							addParticleToRigidBody(particles[particleid - 1], objid);
						}
					}
				}
			}
		}
	}
}


void Simulation::tryAddParticle(float fx, float fy, float critdistance, bool replace, int material) {
	if (fx > 0 && fy > 0 && fx < xbound && fy < ybound) {
		int gxi = (int)(fx / sim_interactiondistancemax);
		int gyi = (int)(fy / sim_interactiondistancemax);
		float canadd = true;
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				int gx = gxi + dx;
				int gy = gyi + dy;
				if (gx < 0 || gy < 0 || gx >= sim_gridcolumns || gy >= sim_gridrows)
					continue;
				Particle* q = grid[gx][gy];
				while (q != nullptr) {
					float distsq = sqrt((q->x - fx) * (q->x - fx) + (q->y - fy) * (q->y - fy));
					Particle* qtmp = q;
					q = q->next;
					if (distsq < critdistance) {
						if (replace) {
							deleteParticle(qtmp->id);
						}
						else {
							canadd = false;
						}
					}
				}
			}
		}
		if (canadd)
			addParticle(fx, fy, flowvelocity, 0.0f, material);
	}
}

void Simulation::addRandomParticle() {
	addParticle(randFloat(xsimarea) + sim_interactiondistancemax, randFloat(ysimarea) + sim_interactiondistancemax, 0.0f, 0.0f, sim_type_water);
}

void Simulation::addParticle(float x, float y, float vx, float vy, int type) {
	//oh shi
	Particle *p = new Particle(x, y, vx, vy, particleid, type, masstable[type]);
	//p->lifespan = randFloat(2.5f) + 1.0f;
	particles.push_back(p);
	//Particle newparticle(randFloat(xbound - 20.0f) + 10.0f, randFloat(ybound - 20.0f) + 10.0f, particleid);
	//particles[particleid] = newparticle;
	//Particle *p = &particles[particleid];
	g_coords.push_back(p->x - drawradius[p->material]);
	g_coords.push_back(p->y - drawradius[p->material]);
	g_coords.push_back(p->x - drawradius[p->material]);
	g_coords.push_back(p->y + drawradius[p->material]);
	g_coords.push_back(p->x + drawradius[p->material]);
	g_coords.push_back(p->y + drawradius[p->material]);
	g_coords.push_back(p->x + drawradius[p->material]);
	g_coords.push_back(p->y - drawradius[p->material]);

	g_uvs.push_back(-1.0);
	g_uvs.push_back(-1.0);
	g_uvs.push_back(-1.0);
	g_uvs.push_back(+1.0);
	g_uvs.push_back(+1.0);
	g_uvs.push_back(+1.0);
	g_uvs.push_back(+1.0);
	g_uvs.push_back(-1.0);


	g_dbglines.push_back(0.0f);
	g_dbglines.push_back(1.0f);
	g_dbglines.push_back(1.0f);
	g_dbglines.push_back(1.0f);

	g_pressures.push_back(0.0f);
	g_pressures.push_back(0.0f);
	g_pressures.push_back(0.0f);
	g_pressures.push_back(0.0f);

	for (int i = 0; i < 4; i++) {
		g_colors.push_back(colortable[type][0]);
		g_colors.push_back(colortable[type][1]);
		g_colors.push_back(colortable[type][2]);
	}

	int nx = (int)(p->x / sim_interactiondistancemax);
	int ny = (int)(p->y / sim_interactiondistancemax);

	p->gridx = nx;
	p->gridy = ny;
	p->next = grid[p->gridx][p->gridy];
	grid[p->gridx][p->gridy] = p;
	if (p->next != nullptr)
		p->next->prev = p;
	p->prev = nullptr;
	particleid++;
}

void Simulation::deleteParticle(int id) {
	Particle *p = particles[id];
	Rigidbody* rb = nullptr;
	if (p->rigidbodyid != -1)
		rb = objects[p->rigidbodyid];
	int index = p->rigidbodyindex;

	if (p->prev != nullptr)
		p->prev->next = p->next;
	else
		grid[p->gridx][p->gridy] = p->next;

	if (p->next != nullptr)
		p->next->prev = p->prev;

	delete particles[id];
	if (id < particleid - 1) {
		memcpy(&g_coords[id * 8], &g_coords[(particleid - 1)*8], 8 * sizeof(float));
		memcpy(&g_uvs[id * 8], &g_uvs[(particleid - 1)*8], 8 * sizeof(float));
		memcpy(&g_dbglines[id * 4], &g_dbglines[(particleid - 1)*4], 4 * sizeof(float));
		memcpy(&g_pressures[id * 4], &g_pressures[(particleid - 1)*4], 4 * sizeof(float));
		memcpy(&g_colors[id * 12], &g_colors[(particleid - 1) * 12], 12 * sizeof(unsigned char));

		particles[id] = particles[particleid - 1];
		particles[id]->id = id;
	}

	for (int i = 0; i < 8; i++) {
		g_coords.pop_back();
		g_uvs.pop_back();
	}

	for (int i = 0; i < 4; i++) {
		g_dbglines.pop_back();
		g_pressures.pop_back();
	}

	for (int i = 0; i < 12; i++) {
		g_colors.pop_back();
	}

	if (rb != nullptr) {
		if (index < rb->numberofparticles - 1) {
			rb->particles[index] = rb->particles[rb->numberofparticles - 1];
			rb->particles[index]->rigidbodyindex = index;
			rb->coordinates[index] = rb->coordinates[rb->numberofparticles - 1];
		}
		rb->particles.pop_back();
		rb->coordinates.pop_back();
		rb->numberofparticles--;
		rb->modified = true;
	}

	particles.pop_back();
	particleid--;

	//todo update grid
}

void Simulation::addParticleToRigidBody(Particle* p, int rbid) {
	p->rigidbodyid = rbid;
	Rigidbody* object = objects[rbid];
	p->rigidbodyindex = object->numberofparticles;
	object->particles.push_back(p);
	object->coordinates.push_back(Coord(0.0f, 0.0f));
	object->numberofparticles++;
	object->modified = true;
}

void Simulation::saveToFile(std::string name) {
	std::ofstream file(name, std::ios::out | std::ios::binary | std::ios::trunc);
	int version = 1;
	file.write((char*)&version, sizeof(int));
	file.write((char*)&this->particleid, sizeof(int));

	for (int i = 0; i < particleid; i++) {
		Particle *p = particles[i];

		file.write((char*)&p->x, sizeof(float));
		file.write((char*)&p->y, sizeof(float));
		file.write((char*)&p->vx, sizeof(float));
		file.write((char*)&p->vy, sizeof(float));
		file.write((char*)&p->material, sizeof(int));
	}
	file.close();
}

void Simulation::loadFromFile(std::string name) {
	this->clear();
	std::ifstream file(name, std::ios::in | std::ios::binary);
	int version;
	int size;
	if (file.is_open())
	{
		file.read((char*)&version, sizeof(int));
		file.read((char*)&size, sizeof(int));
		for (int i = 0; i < size; i++) {
			float x, y, vx, vy;
			int elem;
			file.read((char*)&x, sizeof(float));
			file.read((char*)&y, sizeof(float));
			file.read((char*)&vx, sizeof(float));
			file.read((char*)&vy, sizeof(float));
			file.read((char*)&elem, sizeof(float));
			addParticle(x, y, vx, vy, elem);
		}

		file.close();
	}
}

void Simulation::clear() {
	for (int i = particleid - 1; i >= 0; i--) {
		deleteParticle(i);
	}
}

float Simulation::randFloat(float max) {
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max));
}

float Simulation::randFloatNormal() {
	float f = rngLUT[rngLUTindex];
	rngLUTindex = (rngLUTindex + 1) % rngLUTsize;
	return f;
}