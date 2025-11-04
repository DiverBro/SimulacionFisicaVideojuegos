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
	ParticleSystem() {
		//GESTIONA CONJUNTO DE PARTICULAS
		fP = new FuenteParticulas();
	};
	~ParticleSystem() {
		for (Particle* p : part) {
			delete p;
		}
		part.clear();
	};
	void clearParticles() {
		for (Particle* p : part) {
			delete p;
		}
		part.clear();
	};
	void update(double t, int p, Vector3D pos, bool force = true);
	void addForce(Particle* p, double t);
	void forceVertex(ForceGenerator* f);
	void clearForceVertex();
	void changeArea(Vector3D aMin, Vector3D aMax) { areaMin = aMin; areaMax = aMax; };
};

