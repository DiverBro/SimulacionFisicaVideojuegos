#include "WindGenerator.h"
WindGenerator::WindGenerator(Vector3D windVelocity, float k1, float k2)
	: mWindVelocity(windVelocity), mK1(k1), mK2(k2) {
}

// Aplica fuerza a cada particula por separado
void WindGenerator::applyForce(class Particle* p, float t)
{
	if (!p || !p->windAffects()) return;

	Vector3D relVel = mWindVelocity - p->getVel();
	Vector3D force = relVel * mK1;

	if (mK2 != 0.0f)
	{
		float mag = sqrt(pow(relVel.getX(), 2) + pow(relVel.getY(), 2) + pow(relVel.getZ(), 2));
		force = force + (relVel * (mK2 * mag));
	}

	p->addForce(force, t); // actualiza velocidad: v += F/m * t
}

void WindGenerator::applyForce(physx::PxRigidDynamic* p, float t)
{
	if (!p) return;

	Vector3D relVel = mWindVelocity - p->getLinearVelocity();
	Vector3D force = relVel * mK1;

	if (mK2 != 0.0f)
	{
		float mag = sqrt(pow(relVel.getX(), 2) + pow(relVel.getY(), 2) + pow(relVel.getZ(), 2));
		force = force + (relVel * (mK2 * mag));
	}

	p->addForce(physx::PxVec3(float(force.getX()), float(force.getY()), float(force.getZ())) * t); // actualiza velocidad: v += F/m * t
}

void WindGenerator::setWindVelocity(const Vector3D& windVel) { mWindVelocity = windVel; }
Vector3D WindGenerator::getWindVelocity() { return mWindVelocity; }

void WindGenerator::setCoefficients(float k1, float k2) { mK1 = k1; mK2 = k2; }
float WindGenerator::getK1() const { return mK1; }
float WindGenerator::getK2() const { return mK2; }