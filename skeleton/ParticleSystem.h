#pragma once
#include "FuenteParticulas.h"
#include "ForceGenerator.h"
#include <vector>
#include <vector>

class ParticleSystem
{
private:
	enum type {
		MANGUERA,
		NIEBLA,
		HUMO
	};
	type ty;
	FuenteParticulas* fP;
	std::vector<ForceGenerator*> forces;
	std::vector<Particle*> part;
	Vector3D areaMin = Vector3D(-20, -20, -20);
	Vector3D areaMax = Vector3D(20, 25, 20);
public:
	ParticleSystem(int t) {
		//GESTIONA CONJUNTO DE PARTICULAS
		fP = new FuenteParticulas();
		switch (t) {
		case 0: { ty = MANGUERA;break; }
		case 1: { ty = NIEBLA;break; }
		case 2: { ty = HUMO;break; }
		}
	};
	~ParticleSystem() {
		for (Particle* p : part) {
			delete p;
		}
		part.clear();
	};
	void update(double t);
	void addForce(Particle* p, double t);
	void forceVertex(ForceGenerator* f);
	void clearForceVertex();
	void changeArea(Vector3D aMin, Vector3D aMax) { areaMin = aMin; areaMax = aMax; };
};

