#include "ParticleSystem.h"
#include <iostream>

void ParticleSystem::update(double t, int p, Vector3D pos, bool windWillAffect, bool expWillAffect, bool springWillAffect)
{
	switch (p) {
	case 0: { ty = MANGUERA; break; }
	case 1: { ty = NIEBLA; break; }
	case 2: { ty = HUMO; break; }
	case 3: { ty = ANIM; break; }
	}
	switch (ty) {
	case MANGUERA: part.push_back(fP->generaParticulaManguera(windWillAffect, expWillAffect, springWillAffect)); break;
	case NIEBLA:   part.push_back(fP->generaParticulaNiebla(windWillAffect, expWillAffect, springWillAffect)); break;
	case HUMO:     part.push_back(fP->generaParticulaHumo(pos, windWillAffect, expWillAffect, springWillAffect)); break;
	case ANIM: {
		part.push_back(fP->generaParticulaAnimViento(pos, windWillAffect, expWillAffect, springWillAffect));
		break;
	}
	}
	for (auto it = part.begin(); it != part.end(); ) {
		Particle* p = *it;

		addForce(p, t);
		p->integ(t);
		p->setTiempo(p->getTiempo() - t);

		if (p->getTiempo() <= 0) {
			delete p;
			it = part.erase(it);
		}
		else {
			++it;
		}
	}

	if (explosion) {
		if (explosion->getTau() > -1)
		{
			explosion->updateTime(t);
			if (explosion->getTime() >= explosion->getTau()) {
				explosion->resetExplosion();
				auto it = std::find(forces.begin(), forces.end(), explosion);
				if (it != forces.end()) {
					delete* it;
					forces.erase(it);
				}
				explosion = nullptr;
			}
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

void ParticleSystem::forceVertex(std::string tipo, Vector3D pos, Vector3D velOD, float intensityOK, float radOL, float time, Particle* spring)
{
	ForceGenerator* f;
	if (tipo == "explosion") {
		explosion = new ExplosionGenerator(pos, intensityOK, radOL, time);
		f = explosion;
	}
	else if (tipo == "wind") {
		f = new WindGenerator(velOD, 1, 0);
	}
	else if (tipo == "spring") {
		f = new SpringGenerator(pos, intensityOK, radOL, spring);
	}
	else if (tipo == "flotability") {
		f = new FlotabilityGenerator(0, 0, 1, 1000, 10);
	}
	forces.push_back(f);
}

void ParticleSystem::clearForceVertex()
{
	for (ForceGenerator* f : forces) {
		delete f;
	}
	forces.clear();
}
