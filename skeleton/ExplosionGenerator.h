#pragma once
#include "ForceGenerator.h"

class ExplosionGenerator : public ForceGenerator
{
public:
	ExplosionGenerator(Vector3D center, float intensity, float radius, float tau);

	void updateTime(float deltaTime);

	void resetExplosion();

	void applyForce(class Particle* p, float t) override;

	float getTau() { return mTau; };
	float getTime() { return mTime; };

private:
	Vector3D mCenter;
	float mK;
	float mR;
	float mTau;
	float mTime;
};

