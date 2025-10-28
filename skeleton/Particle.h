#pragma once

#include <PxPhysicsAPI.h> //Para usar Vector3
#include "RenderUtils.hpp" //Para RenderItem
#include "Vector3D.h" //TODO: cambiar luego

#include <chrono> //Para el delay en la integracion 
#include <thread>

using namespace physx;


class Particle
{
public:
	Particle(PxVec3 pos, PxVec3 vel, Vector4 color, PxVec3 ace, double masa, double dumping, double tiempo, double size);
	~Particle();

	void integrateEuler(double t);

	inline RenderItem* getRenderItem() { return renderItem; }
	inline PxVec3 getPose() { return pose->p; }
	inline PxVec3 getVel() { return vel; }
	inline PxVec3 getAce() { return ace; }
	inline double getMasa() { return masa; }
	inline double getDumping() { return dumping; }
	inline double getTiempo() { return tiempo; }
	inline bool isActivo() { return activo; }

private:

	RenderItem* renderItem;
	PxTransform* pose; 
	PxVec3 vel; 
	PxVec3 ace; 
	double masa; 
	double dumping;
	double tiempo; 
	bool activo = false;
};