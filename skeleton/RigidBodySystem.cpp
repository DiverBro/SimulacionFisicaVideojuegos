#include "RigidBodySystem.h"
#include <iostream>

void RigidBodySystem::addForce(PxRigidDynamic* p, double t)
{
	PxTransform pos = p->getGlobalPose();
	if (pos.p.x >= areaMin.getX() && pos.p.x <= areaMax.getX() &&
		pos.p.y >= areaMin.getY() && pos.p.y <= areaMax.getY() &&
		pos.p.z >= areaMin.getZ() && pos.p.z <= areaMax.getZ())
	{
		for (ForceGenerator* fG : forces)
			fG->applyForce(p, (float)t);
	}
}

void RigidBodySystem::forceVertex(std::string tipo, Vector3D pos, Vector3D velOD, float intensityOK, float radOL, float time)
{
	ForceGenerator* f;
	if (tipo == "explosion") {
		explosion = new ExplosionGenerator(pos, intensityOK, radOL, time);
		f = explosion;
	}
	else if (tipo == "wind") {
		f = new WindGenerator(velOD, 1, 0);
	}
	else if (tipo == "spring") {
		f = new SpringGenerator(pos, intensityOK, pos - velOD, radOL);
	}
	forces.push_back(f);
}

void RigidBodySystem::clearForceVertex()
{
	for (ForceGenerator* f : forces) {
		delete f;
	}
	forces.clear();
}
