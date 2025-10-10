#include "Particle.h"
using namespace physx;

Particle::Particle(Vector3D pos, Vector3D vel, Vector3D ac, float dumping) : pose(pos.getX(), pos.getY(), pos.getZ()), vel(vel), ac(ac), dumping(dumping)
{
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &pose, Vector4(1.0f, 0.0f, 1.0f, 1.0f));
}

void Particle::integrate(double t)
{
	vel = vel * pow(dumping, t) + (ac * t);
	pose.p.x += vel.getX() * t;
	pose.p.y += vel.getY() * t;
	pose.p.z += vel.getZ() * t;

	/*if (pose.p.x <= -50 || pose.p.z <= -50) {
		pose = { 0, 0, 0 };
	}*/
}
