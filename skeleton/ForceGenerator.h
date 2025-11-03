#pragma once
#include "Vector3D.h"
#include "Particle.h"
#include <cmath>

class ForceGenerator
{
public:
	ForceGenerator(Vector3D windVelocity, float k1, float k2 = 0.0f)
		: mWindVelocity(windVelocity), mK1(k1), mK2(k2) {
	}

	// Aplica fuerza a cada particula por separado
	void applyForce(class Particle* p, float t)
	{
		if (!p) return;

		Vector3D relVel = mWindVelocity - p->getVel();
		Vector3D force = relVel * mK1;

		if (mK2 != 0.0f)
		{
			float mag = sqrt(pow(relVel.getX(), 2) + pow(relVel.getY(), 2) + pow(relVel.getZ(), 2));
			force = force + (relVel * (mK2 * mag));
		}

		p->addForce(force, t); // actualiza velocidad: v += F/m * t
	}

	void setWindVelocity(const Vector3D& windVel) { mWindVelocity = windVel; }
	Vector3D getWindVelocity() const { return mWindVelocity; }

	void setCoefficients(float k1, float k2 = 0.0f) { mK1 = k1; mK2 = k2; }
	float getK1() const { return mK1; }
	float getK2() const { return mK2; }

private:
	Vector3D mWindVelocity;
	float mK1;
	float mK2;
};
