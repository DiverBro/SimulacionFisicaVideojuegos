#include "Proyectil.h"
using namespace physx;

Proyectil::Proyectil(Vector3D pos, Vector3D vel, Vector3D grav, int masa, float dumping) : pose(pos.getX(), pos.getY(), pos.getZ()), vel(vel), grav(grav),
masa(masa), dumping(dumping)
{
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &pose, Vector4(1.0f, 0.0f, 1.0f, 1.0f));
}
