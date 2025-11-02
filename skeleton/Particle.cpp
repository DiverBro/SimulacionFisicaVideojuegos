#include "Particle.h"
using namespace physx;

Particle::Particle(Vector3D pos, Vector3D vel, Vector3D ace, float dumping, float masa, float grav, Vector4 col) : pose(pos.getX(), pos.getY(), pos.getZ()), vel(vel),
ace(ace), dumping(dumping), masa(masa), grav(grav)
{
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &pose, col);
}

void Particle::integ(double t)
{
	vel = vel * pow(dumping, t) + (ace * t);
	pose.p += Vector3(vel.getX(), vel.getY(), vel.getZ()) * t;
}