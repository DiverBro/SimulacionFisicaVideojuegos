#pragma once
#include <fstream>
#include <vector>
#include <string>
#include "Vector3D.h"
#include "Proyectil.h"
#include "ParticleSystem.h"
#include "RigidBodySystem.h"
#include "WindGenerator.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
class Mapa
{
private:
	std::vector<RenderItem*> objetos;
	float metaXBegin;
	float metaXEnd;
	float metaYBegin;
	float metaYEnd;
	ParticleSystem* partSys;
	RigidBodySystem* rigidSysWind;
	RigidBodySystem* rigidSysExp;
	Vector3D posIniPr = Vector3D(0, 0, 0);
	bool victoria = false;
	double tiempo = 0;
	RenderItem* arrow;
	physx::PxVec3 initialArrowPos;
	Vector3D posViento = Vector3(-1, -1, -1);
	PxPhysics* gPhysics;
	PxScene* gScene;
	PxRigidDynamic* proyectil;
	std::vector<std::string> map;
	Vector3D startPos;
	Vector3D blockSize;
	int i;
public:
	Mapa(int i, Vector3D startPos, Vector3D blockSize, PxRigidDynamic* ball, PxPhysics* gPhysics, PxScene* gScene);
	void update(double t, float angle);
	void createMap(std::string filename, Vector3D startPos, Vector3D blockSize);
	void changeMap(Vector3D startPos, Vector3D blockSize);
	void resetLevel(Vector3D startPos, Vector3D blockSize);
	void fillMaps(const std::string& filename);
};

