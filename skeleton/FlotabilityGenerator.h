#pragma once
#include "ForceGenerator.h"
using namespace physx;
class FlotabilityGenerator : public ForceGenerator
{
public:
	FlotabilityGenerator(float h, float h0, float _volume, float _liquid_density, float _height)
		: h(h), h0(h0), _volume(_volume), _liquid_density(_liquid_density), _height(_height) {
		Vector3D pos = Vector3D(0, -100, 0);
		RenderItem* rnd = new RenderItem(CreateShape(PxBoxGeometry(100, 100, 100)),
			new PxTransform(pos.getX(), pos.getY(), pos.getZ()), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	}

	void applyForce(class Particle* p, float t) override;
private:
	float immersed;
	float h0;
	float h;
	float _height;
	float _liquid_density;
	float _volume;
};

