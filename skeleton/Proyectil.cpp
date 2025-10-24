#include "Proyectil.h"
using namespace physx;

Proyectil::Proyectil(Vector3D pos, Vector3D vel, Vector3D grav, float damping)
	: pose(pos.getX(), pos.getY(), pos.getZ()), velSim(vel), grav(grav), damping(damping)
{
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &pose, Vector4(1.0f, 0.0f, 1.0f, 1.0f));
}

void Proyectil::masaSimulada()
{
	masaSim = masaReal * pow((velReal.getX() / velSim.getX()), 2);
}

