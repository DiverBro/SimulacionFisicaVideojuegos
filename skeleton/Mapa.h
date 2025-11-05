#pragma once
#include <fstream>
#include <vector>
#include <string>
#include "Vector3D.h"
#include "Proyectil.h"
#include "ParticleSystem.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
class Mapa
{
private:
	std::vector<RenderItem*> objetos;
	Proyectil* proyectil;
	float metaXBegin;
	float metaXEnd;
	float metaYBegin;
	float metaYEnd;
	ParticleSystem* partSys;
	Vector3D posIniPr = Vector3D(0, 0, 0);
	bool victoria = false;
	double tiempo = 0;
	RenderItem* arrow;
	physx::PxVec3 initialArrowPos;
public:
	Mapa(const std::string& filename, Vector3D startPos, Vector3D blockSize, Proyectil* pr);
	void update(double t, float angle);
};

