#pragma once
#include "Particle.h"


class Proyectil : public Particle
{
public:

	Proyectil(Vector3D pos, Vector3D dir, float velReal, float mReal, float velSim, float dumping = 1.0, float grav = 9.8);

	virtual ~Proyectil() {};

	float masaSim(float mReal, float velSim, float velReal);
	float gravSim(float grav, float velSim, float velReal);
private:
	float vel;
};
