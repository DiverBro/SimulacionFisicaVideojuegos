#pragma once
#include "Vector3D.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

class Particle
{
public:                                                                                                    //MASA COMO PARAM
	Particle(Vector3D pos, Vector3D vel, Vector3D ace, float dumping, float tam = 1.0f, float tiempo = 4.0f, float masa = 10.0f, Vector3D grav = Vector3D(0, -9.8f, 0), Vector4 col = { 1.0f, 0.0f, 0.0f, 1.0f });
	~Particle() { DeregisterRenderItem(renderItem); renderItem = nullptr; };

	virtual void integ(double t);//METODO INTEGRACION
	Vector3D getAce() { return ace; };
	Vector3D getVel() { return vel; };
	physx::PxVec3 getPose() { return pose.p; };
	float getDumping() { return dumping; };
	float getMasa() { return masa; };
	float getTiempo() { return tiempo; };
	void setVel(Vector3D v) { vel = v; };
	void setPose(physx::PxVec3 pos) { pose.p = pos; };
	void setAce(Vector3D a) { ace = a; };
	void setTiempo(float t) { tiempo = t; };
	void addForce(Vector3D force, double t);
	void clearForces();
	void setForceAffects(bool f) { forceAffects = f; };
	bool getForceAffects() { return forceAffects; };
private:
	bool forceAffects;
	Vector3D vel;
	physx::PxTransform pose;
	RenderItem* renderItem;
	float dumping;
	Vector3D ace;
	Vector3D grav;
	Vector3D forceAcum = Vector3D(0, 0, 0);
	float masa;
	float tiempo;
};

