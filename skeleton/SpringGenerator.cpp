#include "SpringGenerator.h"

SpringGenerator::SpringGenerator(Vector3D pos, float k, Vector3D d, float l) : k(k), d(d), l(l)
{
	RenderItem* rnd = new RenderItem(CreateShape(PxBoxGeometry(2, 2, 2)),
		new PxTransform(pos.getX(), pos.getY(), pos.getZ()), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
}

void SpringGenerator::applyForce(Particle* p, float t)
{
	if (!p || !p->springAffects()) return;

	float length = sqrt(pow(d.getX(), 2) + pow(d.getY(), 2) + pow(d.getZ(), 2));  

	if (length < 1e-6f) return;    

	Vector3D dir = d / length;     

	float x = length - l;          

	Vector3D force = dir * (-k * x);

	p->addForce(force, t);
}
