#include "ExplosionGenerator.h"
#include <iostream>
ExplosionGenerator::ExplosionGenerator(Vector3D center, float intensity, float radius, float tau)
	: mCenter(center), mK(intensity), mR(radius), mTau(tau), mTime(0.0f) {
}

void ExplosionGenerator::updateTime(float deltaTime)
{
	mTime += deltaTime;
}

void ExplosionGenerator::resetExplosion()
{
	mTime = 0.0f;
}

void ExplosionGenerator::applyForce(class Particle* p, float t)
{
	if (!p || !p->expAffects()) return;

	Vector3D pos = p->getPose();
	Vector3D diff = pos - mCenter;

	float r = sqrt(diff.getX() * diff.getX() +
		diff.getY() * diff.getY() +
		diff.getZ() * diff.getZ());

	if (r < mR && r > 1e-6f)
	{
		Vector3D dir = diff * (1.0f / r);

		float decay = exp(-mTime / mTau);

		float magnitude = (mK / (r * r)) * decay;

		Vector3D force = dir * magnitude;

		p->addForce(force, t);
	}
}

void ExplosionGenerator::applyForce(physx::PxRigidDynamic* p, float t)
{
	if (!p) return;

	physx::PxTransform pos = p->getGlobalPose();
	physx::PxVec3 diff = pos.p - physx::PxTransform({ float(mCenter.getX()), float(mCenter.getY()), float(mCenter.getZ()) }).p;

	float r = sqrt(diff.x * diff.x +
		diff.y * diff.y +
		diff.z * diff.z);

	if (r < mR && r > 1e-6f)
	{
		Vector3D dir = diff * (1.0f / r);

		float decay = exp(-mTime / mTau);

		float magnitude = (mK / (r * r)) * decay;

		Vector3D force = dir * magnitude;

		p->addForce(physx::PxVec3(float(force.getX()), float(force.getY()), float(force.getZ())) * t);
	}
}