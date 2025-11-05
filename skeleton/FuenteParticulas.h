#pragma once
#include "Particle.h"
#include <random>

class FuenteParticulas
{
	std::mt19937 gen;
public:
	FuenteParticulas() {};
	~FuenteParticulas() {};

	//GENERADOR DE MANGUERA
	Particle* generaParticulaManguera(bool windWillAffect = true, bool expWillAffect = true) {
		//DISTRIBUCION NORMAL
		std::uniform_real_distribution<double> dir(0, 1);
		Vector3D direccion = Vector3D(dir(gen), dir(gen), dir(gen));
		std::uniform_real_distribution<double> velX(1, 7);
		std::uniform_real_distribution<double> velY(10, 20);
		std::uniform_real_distribution<double> velZ(1, 7);
		Particle* p = new Particle(Vector3D(0, 0, 0), Vector3D(direccion.getX() * velX(gen), direccion.getY() * velY(gen), direccion.getZ() * velZ(gen)), Vector3D(0, -9.8, 0), 0.999);
		p->setWindAffects(windWillAffect);
		p->setExpAffects(expWillAffect);
		return p;
	};

	//GENERADO DE NIEBLA
	Particle* generaParticulaNiebla(bool windWillAffect = true, bool expWillAffect = true) {
		//DISTRIBUCION NORMAL
		std::normal_distribution<double> posX(10, 2);
		std::normal_distribution<double> posY(30, 2);
		std::normal_distribution<double> posZ(10, 2);
		std::normal_distribution<double> velX(1, 1);
		std::normal_distribution<double> velY(1, 1);
		std::normal_distribution<double> velZ(1, 1);
		Particle* p = new Particle(Vector3D(posX(gen), posY(gen), posZ(gen)), Vector3D(velX(gen), velY(gen), velZ(gen)),
			Vector3D(0, 0, 0), 0.999, 0.2f, 2.0f, 10.0f, Vector3D(0, -0.8f, 0), { 0.5f, 0.5f, 0.5f, 0.5f });
		p->setWindAffects(windWillAffect);
		p->setExpAffects(expWillAffect);
		return p;
	};

	//GENERADOR DE HUMO
	Particle* generaParticulaHumo(Vector3D pos = Vector3D(0, 0, 0), bool windWillAffect = true, bool expWillAffect = true) {
		//DISTRIBUCION NORMAL
		std::normal_distribution<double> velX(0, 0.3);
		std::normal_distribution<double> velY(6, 1);
		std::normal_distribution<double> velZ(0, 0.3);
		Particle* p = new Particle(pos, Vector3D(velX(gen), velY(gen), velZ(gen)),
			Vector3D(0, 0, 0), 0.999, 0.4f, 8.0f, 10.0f, Vector3D(0, -0.1f, 0), { 0.5f, 0.5f, 0.5f, 0.8f });
		p->setWindAffects(windWillAffect);
		p->setExpAffects(expWillAffect);
		return p;
	};
};

