#include "FlotabilityGenerator.h"

void FlotabilityGenerator::applyForce(class Particle* p, float t) {
	h = p->getPose().y;
	if (h - h0 > _height * 0.5) {
		immersed = 0.0;
	}
	else if (h0 - h > _height * 0.5) {
		immersed = 1.0;
	}
	else {
		immersed = (h0 - h) / _height + 0.5;
	}
	Vector3D f = Vector3D(0, 0, 0);
	f.setY(_liquid_density * _volume * immersed * 9.8);
	p->addForce(f / p->getMasa(), t);
}