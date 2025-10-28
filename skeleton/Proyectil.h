#pragma once

#include <PxPhysicsAPI.h> //Para usar Vector3
#include "RenderUtils.hpp" //Para RenderItem
#include "Vector3D.h" //TODO: cambiar luego
#include <vector> //Para el vector de proyectiles
#include "Particle.h" 

using namespace physx;

class Proyectil : public Particle
{
public:
	Proyectil(PxVec3 pos, PxVec3 vel, Vector4 color, PxVec3 ace, double masa, double dumping, double tiempo);

	enum class ProyectilType { Bullet, CanonBall, Bubble };

	void shoot();

	~Proyectil() {};

	inline PxVec3 getGravity() { return grav; }

	inline void setGravity(PxVec3 newGravity) { grav = newGravity; }


private:
	PxVec3 grav;
	Vector4 color;
};
