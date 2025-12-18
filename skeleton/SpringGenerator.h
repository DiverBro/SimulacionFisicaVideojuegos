#pragma once
#include "ForceGenerator.h"
using namespace physx;
class SpringGenerator : public ForceGenerator
{
public:
	SpringGenerator(Vector3D pos, float k, float l, Particle* attach = nullptr);

	void applyForce(class Particle* p, float t) override;
private:
	float k, l;
	Vector3D pos;
	Particle* attach = nullptr;;
};

