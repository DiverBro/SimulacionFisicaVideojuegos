#pragma once
#include "ForceGenerator.h"
class WindGenerator : public ForceGenerator
{
public:
	WindGenerator(Vector3D windVelocity, float k1, float k2 = 0.0f);

	void applyForce(class Particle* p, float t) override;
	void applyForce(physx::PxRigidDynamic* p, float t) override;

	void setWindVelocity(const Vector3D& windVel);
	Vector3D getWindVelocity();

	void setCoefficients(float k1, float k2 = 0.0f);
	float getK1() const;
	float getK2() const;

private:
	Vector3D mWindVelocity;
	float mK1;
	float mK2;
};

