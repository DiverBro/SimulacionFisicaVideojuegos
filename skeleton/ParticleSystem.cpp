#include "ParticleSystem.h"
#include <iostream>

void ParticleSystem::update(double t, int p, Vector3D pos, bool windWillAffect, bool expWillAffect)
{
	switch (p) {
	case 0: { ty = MANGUERA; break; }
	case 1: { ty = NIEBLA; break; }
	case 2: { ty = HUMO; break; }
	case 3: { ty = ANIM; break; }
	}
	switch (ty) {
	case MANGUERA: part.push_back(fP->generaParticulaManguera(windWillAffect, expWillAffect)); break;
	case NIEBLA:   part.push_back(fP->generaParticulaNiebla(windWillAffect, expWillAffect)); break;
	case HUMO:     part.push_back(fP->generaParticulaHumo(pos, windWillAffect, expWillAffect)); break;
	case ANIM:     part.push_back(fP->generaParticulaHumo(pos, windWillAffect, expWillAffect)); break;
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

	if (explosion) {
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

void ParticleSystem::forceVertex(std::string tipo, Vector3D pos, Vector3D vel, float intensity, float rad, float time)
{
	ForceGenerator* f;
	if (tipo == "explosion") {
		explosion = new ExplosionGenerator(pos, intensity, rad, time);
		f = explosion;
	}
	else if (tipo == "wind") {
		f = new WindGenerator(vel, 1, 0);
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
