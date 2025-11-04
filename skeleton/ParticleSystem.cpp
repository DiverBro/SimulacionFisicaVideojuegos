#include "ParticleSystem.h"
#include <iostream>

void ParticleSystem::update(double t, int p, Vector3D pos, bool forceWillAffect)
{
	switch (p) {
	case 0: { ty = MANGUERA;break; }
	case 1: { ty = NIEBLA;break; }
	case 2: { ty = HUMO;break; }
	}
	switch (ty) {
	case MANGUERA: part.push_back(fP->generaParticulaManguera(forceWillAffect)); break;
	case NIEBLA:   part.push_back(fP->generaParticulaNiebla(forceWillAffect)); break;
	case HUMO:     part.push_back(fP->generaParticulaHumo(pos, forceWillAffect)); break;
	}
	for (Particle* p : part) {
		if (p->getForceAffects())
		{
			addForce(p, t);
		}
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
