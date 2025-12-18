#include "SpringGenerator.h"

SpringGenerator::SpringGenerator(Vector3D pos, float k, float l, Particle* a) : k(k), pos(pos), l(l)
{
	if (a == nullptr)
		RenderItem* rnd = new RenderItem(CreateShape(PxBoxGeometry(2, 2, 2)),
			new PxTransform(pos.getX(), pos.getY(), pos.getZ()), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	else
		attach = a;
}

void SpringGenerator::applyForce(Particle* p, float t)
{
	if (!p || !p->springAffects()) return;

	if (attach == nullptr)
	{
		PxVec3 anchor(pos.getX(), pos.getY(), pos.getZ());
		Vector3D d = p->getPose() - anchor;

		float length = sqrt(pow(d.getX(), 2) + pow(d.getY(), 2) + pow(d.getZ(), 2));
		if (length < 1e-6f) return;

		if (length > l)
		{
			Vector3D dir = d / length;
			float x = length - l;

			Vector3D force = dir * (-k * x);
			p->addForce(force, t);
		}
	}
	else {
		PxVec3 anchor(attach->getPose());
		Vector3D d = p->getPose() - anchor;

		float length = sqrt(pow(d.getX(), 2) + pow(d.getY(), 2) + pow(d.getZ(), 2));
		if (length < 1e-6f) return;

		if (length > l)
		{
			Vector3D dir = d / length;
			float x = length - l;

			Vector3D force = dir * (-k * x);
			p->addForce(force, t);
		}

		/*anchor = p->getPose();
		d = attach->getPose() - anchor;

		length = sqrt(pow(d.getX(), 2) + pow(d.getY(), 2) + pow(d.getZ(), 2));
		if (length < 1e-6f) return;

		if (length > l)
		{
			Vector3D dir = d / length;
			float x = length - l;

			Vector3D force = dir * (-k * x);
			attach->addForce(force, t);
		}*/
	}
}

