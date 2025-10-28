#include "Particle.h"

Particle::Particle(PxVec3 pos, PxVec3 vel, Vector4 color, PxVec3 ace, double masa, double dumping, double tiempo, double size) :
	vel(vel), ace(ace), masa(masa), dumping(dumping), tiempo(tiempo)
{
	//Creamos la forma con la geometria
	PxShape* sphere0 = CreateShape(PxSphereGeometry(size));

	pose = new PxTransform(PxVec3(pos.x, pos.y, pos.z));
	renderItem = new RenderItem(sphere0, pose, color);

	RegisterRenderItem(renderItem);
}

Particle::~Particle()
{
	if (renderItem != nullptr)
	{
		DeregisterRenderItem(renderItem);
		delete renderItem;
		renderItem = nullptr;
	}

	if (pose != nullptr)
	{
		delete pose;
		pose = nullptr;
	}
}

void Particle::integrateEuler(double t)
{
	pose->p += vel * t;
	vel += ace * t;
	vel *= pow(dumping, t);

}