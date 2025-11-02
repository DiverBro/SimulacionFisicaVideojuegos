#include "ParticleSystem.h"

void ParticleSystem::update(double t)
{
	switch (ty) {
	case MANGUERA:
	{
		part.push_back(fP->generaParticulaManguera());
		break;
	}
	case NIEBLA:
	{
		part.push_back(fP->generaParticulaNiebla());
		break;
	}
	case HUMO:
	{
		part.push_back(fP->generaParticulaHumo());
		break;
	}
	}
	for (Particle* p : part) {
		p->integ(t);
		p->setTiempo(p->getTiempo() - t);
		if (p->getTiempo() <= 0) {
			part.erase(std::remove(part.begin(), part.end(), p), part.end());
			delete p;
		}
	}
}
