#pragma once
#include "Vector3D.h"
#include "Particle.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <cmath>

class ForceGenerator
{
public:
	ForceGenerator() {};
	virtual ~ForceGenerator() {};
	virtual void applyForce(class Particle* p, float t) {};
};