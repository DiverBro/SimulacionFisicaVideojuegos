#pragma once
#include "Vector3D.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

class Proyectil
{
private:
	int masa;
	float dumping;
	Vector3D grav;
	Vector3D vel;
	physx::PxTransform pose;
	RenderItem* renderItem;

public:
	Proyectil(Vector3D pos, Vector3D vel, Vector3D grav, int masa, float dumping);
	~Proyectil() { DeregisterRenderItem(renderItem); renderItem = nullptr; };
};

