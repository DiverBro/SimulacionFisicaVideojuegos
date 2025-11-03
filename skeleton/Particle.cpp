#include "Particle.h"
using namespace physx;

Particle::Particle(Vector3D pos, Vector3D vel, Vector3D ace, float dumping, float tam, float tiempo, float masa, Vector3D grav, Vector4 col) : pose(pos.getX(), pos.getY(), pos.getZ()), vel(vel),
ace(ace), dumping(dumping), masa(masa), grav(grav), tiempo(tiempo)
{
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(tam)), &pose, col);
}

void Particle::integ(double t)
{
    Vector3D totalAcc = (forceAcum / masa) + grav + ace;
    vel = vel * pow(dumping, t) + totalAcc * t;
    pose.p += Vector3(vel.getX(), vel.getY(), vel.getZ()) * t;

    clearForces();
}

//GESTION DE FUERZAS TENIENDO EN CUENTA LA MASA
void Particle::addForce(Vector3D force, double t)
{
    forceAcum = forceAcum + force;
}

void Particle:: clearForces() {
	forceAcum = Vector3D(0, 0, 0);
}
