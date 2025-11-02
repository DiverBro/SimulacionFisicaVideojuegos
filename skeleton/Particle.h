#pragma once
#include "Vector3D.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3D pos, Vector3D vel, Vector3D ace, float dumping, float masa = 10.0f, float grav = -9.8f, Vector4 col = { 1.0f, 0.0f, 0.0f, 1.0f });
	~Particle() { DeregisterRenderItem(renderItem); renderItem = nullptr; };

	virtual void integ(double t);
	Vector3D getAce() { return ace; };
	Vector3D getVel() { return vel; };
	physx::PxVec3 getPose() { return pose.p; };
	float getDumping() { return dumping; };
	float getMasa() { return masa; };
	void setVel(Vector3D v) { vel = v; };
	void setPose(physx::PxVec3 pos) { pose.p = pos; };
	void setAce(Vector3D a) { ace = a; };

private:
	Vector3D vel;
	physx::PxTransform pose;
	RenderItem* renderItem;
	float dumping;
	Vector3D ace;
	float grav;
	float masa;
};

