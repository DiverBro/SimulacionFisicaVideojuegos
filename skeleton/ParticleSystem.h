#pragma once
#include "FuenteParticulas.h"
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
	std::vector<Particle*> part;
public:
	ParticleSystem(int t) {
		fP = new FuenteParticulas();
		switch (t) {
		case 0:
		{
			ty = MANGUERA;
			break;
		}
		case 1:
		{
			ty = NIEBLA;
			break;
		}
		case 2:
		{
			ty = HUMO;
			break;
		}
		}
	};
	~ParticleSystem() {};
	void update(double t);
};

