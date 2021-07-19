#include "../Includes.h"
#include "Save.h"
#include "Particle.h"
#include "Simulation.h"
#include "../Life.h"
#include <fstream>
#include <iostream>


void Save::saveToFile(Simulation* sim) {
	char filename[MAX_PATH];
	
	OPENFILENAMEA ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "All Files\0*.*\0\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a file to save";
	ofn.Flags = OFN_DONTADDTORECENT;

	if (!GetSaveFileNameA(&ofn))
	{
		// All this stuff below is to tell you exactly how you messed up above. 
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: return;
		}
	}


	std::ofstream file(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	int version = 2;
	file.write((char*)&version, sizeof(int));

	file.write((char*)&sim->objectid, sizeof(int));
	for (int i = 0; i < sim->objectid; i++) {
		Rigidbody* rb = sim->objects[i];
		file.write((char*)&rb->id, sizeof(int));
		file.write((char*)&rb->vx, sizeof(float));
		file.write((char*)&rb->vy, sizeof(float));
		file.write((char*)&rb->omega, sizeof(float));
		file.write((char*)&rb->brittle, sizeof(bool));
	}

	file.write((char*)&sim->particleid, sizeof(int));
	for (int i = 0; i < sim->particleid; i++) {
		Particle* p = sim->particles[i];

		file.write((char*)&p->id, sizeof(int));
		file.write((char*)&p->rigidbodyid, sizeof(int));
		file.write((char*)&p->x, sizeof(float));
		file.write((char*)&p->y, sizeof(float));
		file.write((char*)&p->vx, sizeof(float));
		file.write((char*)&p->vy, sizeof(float));
		file.write((char*)&p->mass, sizeof(float));
		file.write((char*)&p->material, sizeof(int));
	}

	file.write((char*)&sim->bondid, sizeof(int));
	for (int i = 0; i < sim->bondid; i++) {
		Bond* b = sim->bonds[i];

		file.write((char*)&b->id, sizeof(int));
		file.write((char*)&b->particleA->id, sizeof(int));
		file.write((char*)&b->particleB->id, sizeof(int));
		file.write((char*)&b->rigidbodyid, sizeof(int));
		file.write((char*)&b->bondtype, sizeof(int));
		file.write((char*)&b->length, sizeof(float));
		file.write((char*)&b->variablelength , sizeof(bool));
	}

	file.close();
}
void Save::loadFromFile(Simulation* sim) {

	char filename[MAX_PATH];

	OPENFILENAMEA ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "All Files\0*.*\0\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a file to load";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (!GetOpenFileNameA(&ofn))
	{
		// All this stuff below is to tell you exactly how you messed up above. 
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: return;
		}
	}

	sim->clear();
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	int version;
	int size;
	if (file.is_open())
	{
		file.read((char*)&version, sizeof(int));

		if (version == 1) {
			file.read((char*)&size, sizeof(int));
			for (int i = 0; i < size; i++) {
				float x, y, vx, vy;
				int elem;
				file.read((char*)&x, sizeof(float));
				file.read((char*)&y, sizeof(float));
				file.read((char*)&vx, sizeof(float));
				file.read((char*)&vy, sizeof(float));
				file.read((char*)&elem, sizeof(float));
				sim->addParticle(x, y, vx, vy, elem);
			}
		}
		else if (version == 2) {

			file.read((char*)&size, sizeof(int));
			for (int i = 0; i < size; i++) {
				int id;
				float vx, vy, omega;
				bool brittle;
				file.read((char*)&id, sizeof(int));
				file.read((char*)&vx, sizeof(float));
				file.read((char*)&vy, sizeof(float));
				file.read((char*)&omega, sizeof(float));
				file.read((char*)&brittle, sizeof(bool));
				Rigidbody* rb = sim->addRigidBody(brittle);
				rb->vx = vx;
				rb->vy = vy;
				rb->omega = omega;
			}

			file.read((char*)&size, sizeof(int));
			for (int i = 0; i < size; i++) {
				int id, rigidbodyid, material;
				float x, y, vx, vy, mass;
				file.read((char*)&id, sizeof(int));
				file.read((char*)&rigidbodyid, sizeof(int));
				file.read((char*)&x, sizeof(float));
				file.read((char*)&y, sizeof(float));
				file.read((char*)&vx, sizeof(float));
				file.read((char*)&vy, sizeof(float));
				file.read((char*)&mass, sizeof(float));
				file.read((char*)&material, sizeof(int));
				Particle* p = sim->addParticle(x, y, vx, vy, material);
				p->mass = mass;
				if (rigidbodyid != -1)
					sim->addParticleToRigidBody(p, sim->objects[rigidbodyid]);
			}

			file.read((char*)&size, sizeof(int));
			for (int i = 0; i < size; i++) {
				int id, particleAid, particleBid, rigidbodyid, bondtype;
				float length;
				bool variablelength;
				file.read((char*)&id, sizeof(int));
				file.read((char*)&particleAid, sizeof(int));
				file.read((char*)&particleBid, sizeof(int));
				file.read((char*)&rigidbodyid, sizeof(int));
				file.read((char*)&bondtype, sizeof(int));
				file.read((char*)&length, sizeof(float));
				file.read((char*)&variablelength, sizeof(bool));
				Bond* b = sim->addBond(sim->particles[particleAid], sim->particles[particleBid], bondtype, length);
				b->variablelength = variablelength;
				if (rigidbodyid != -1)
					sim->addBondToRigidBody(b, sim->objects[rigidbodyid]);
			}


			for (int i = 0; i < sim->objectid; i++) {
				sim->objects[i]->initializeUnconstrainedBody();
			}
		}

		file.close();
	}
}