#pragma once
#include "Vector3D.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

class Particle
{
public:
	Particle(Vector3D pos, Vector3D vel, Vector3D ace, float m = 10.0, float dumping = 0.9, float grav = 9.8, float tiempo = 4.0f);
	~Particle() { DeregisterRenderItem(renderItem); renderItem = nullptr; };

	virtual void integ(double t);

	float getT() const { return tiempo; };
	Vector3D getVel() { return vel; };
	Vector3D getPos() { return Vector3D(pose.p.x, pose.p.y, pose.p.z); };
	void setT(float t) { tiempo = t; }
	void setA(Vector3D a) { ace = a; }
		
private:
	Vector3D vel;
	physx::PxTransform pose;
	RenderItem* renderItem;
	float dumping;
	Vector3D ace;
	float m;
	float tiempo;
	float grav;
};