#include "Particle.h"
using namespace physx;

Particle::Particle(Vector3D pos, Vector3D vel, Vector3D ace, float dumping, float m, float grav, float tiempo) : pose(pos.getX(), pos.getY(), pos.getZ()), vel(vel),
ace(ace), dumping(dumping), m(m), grav(grav), tiempo(tiempo)
{
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0f)), &pose, Vector4(1.0f, 1.0f, 0.0f, 1.0f));

}

void Particle::integ(double t)
{
	pose.p.x += vel.getX() * t;
	pose.p.y += vel.getY() * t;
	pose.p.z += vel.getZ() * t;
	vel = vel * pow(dumping, t) + (ace * t);
}