#pragma once
#include "Vector3D.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

class Proyectil
{
private:
	int masaSim;
	int masaReal = 25;
	float damping;
	Vector3D grav;
	Vector3D velSim;
	Vector3D velReal = { 1800, 0, 0 };
	physx::PxTransform pose;
	RenderItem* renderItem;

public:
	Proyectil(Vector3D pos, Vector3D vel, Vector3D grav, float damping);
	~Proyectil() { DeregisterRenderItem(renderItem); renderItem = nullptr; };

	void masaSimulada();
};