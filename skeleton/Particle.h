#pragma once
#include "Vector3D.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3D pos, Vector3D vel, Vector3D ace, float dumping);
	~Particle() { DeregisterRenderItem(renderItem); renderItem = nullptr; };

	virtual void integ(double t);
		
private:
	Vector3D vel;
	physx::PxTransform pose;
	RenderItem* renderItem;
	float dumping;
	Vector3D ace;
};

