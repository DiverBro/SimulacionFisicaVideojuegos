#include "ParticleSystem.h"
#include <iostream>

void ParticleSystem::update(double t)
{
	switch (ty) {
	case MANGUERA: part.push_back(fP->generaParticulaManguera()); break;
	case NIEBLA:   part.push_back(fP->generaParticulaNiebla()); break;
	case HUMO:     part.push_back(fP->generaParticulaHumo()); break;
	}
	for (Particle* p : part) {
		addForce(p, t);
		p->integ(t);
		p->setTiempo(p->getTiempo() - t);
		if (p->getTiempo() <= 0) {
			part.erase(std::remove(part.begin(), part.end(), p), part.end());
			delete p;
		}
	}
}

void ParticleSystem::addForce(Particle* p, double t)
{
	Vector3D pos = p->getPose();
	if (pos.getX() >= areaMin.getX() && pos.getX() <= areaMax.getX() &&
		pos.getY() >= areaMin.getY() && pos.getY() <= areaMax.getY() &&
		pos.getZ() >= areaMin.getZ() && pos.getZ() <= areaMax.getZ())
	{
		for (ForceGenerator* fG : forces)
			fG->applyForce(p, (float)t);
	}
}

void ParticleSystem::forceVertex(ForceGenerator* f)
{
	forces.push_back(f);
}

void ParticleSystem::clearForceVertex()
{
	for (ForceGenerator* f : forces) {
		delete f;
	}
	forces.clear();
}
