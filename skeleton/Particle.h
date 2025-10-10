#pragma once
#include "Vector3D.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3D pos, Vector3D vel, Vector3D ac, float dumping);
	~Particle() { DeregisterRenderItem(renderItem); renderItem = nullptr; };

	void integrate(double t);
private:
	Vector3D vel;
	Vector3D ac;
	physx::PxTransform pose;
	RenderItem* renderItem;
	float dumping;
};

