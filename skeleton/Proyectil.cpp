#include "Proyectil.h"

Proyectil::Proyectil(PxVec3 pos, PxVec3 vel, Vector4 color, PxVec3 ace, double masa, double dumping, double tiempo) :
	Particle(pos, vel, color, gravity, masa, dumping, tiempo, 1.0), grav(ace), color(color) {}

Proyectil::~Proyectil() {}

void Proyectil::shoot()
{
	Particle* part = new Particle(GetCamera()->getTransform().p, getVel(), color, grav, getMasa(), getDumping(), getTiempo(), 1.0);

	PxVec3 dir = PxVec3(GetCamera()->getDir());
}