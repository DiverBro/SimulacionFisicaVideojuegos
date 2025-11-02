#pragma once
#include "Particle.h"


class Proyectil : public Particle
{
public:

	Proyectil(Vector3D pos, Vector3D dir, Vector3D ace, float velReal, float mReal, float velSim, Vector4 col = { 1.0f, 0.0f, 1.0f, 1.0f }, float dumping = 1.0, Vector3D grav = Vector3D(0, 9.8, 0));

	virtual ~Proyectil() {};

	float masaSim(float mReal, float velSim, float velReal);
	float gravSim(float grav, float velSim, float velReal);
};

