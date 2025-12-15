#pragma once
#include "FuenteParticulas.h"
#include "ExplosionGenerator.h"
#include "WindGenerator.h"
#include "SpringGenerator.h"
#include <vector>

class RigidBodySystem
{
private:
	std::vector<ForceGenerator*> forces;
	std::vector<PxRigidDynamic*> rigid;
	Vector3D areaMin = Vector3D(-20, -20, -20);
	Vector3D areaMax = Vector3D(20, 25, 20);
	ExplosionGenerator* explosion = nullptr;
public:
	RigidBodySystem() {

	};
	~RigidBodySystem() {
		for (PxRigidDynamic* p : rigid) {
			//delete p;
		}
		rigid.clear();
	};
	void clearParticles() {
		for (PxRigidDynamic* p : rigid) {
			//delete p;
		}
		rigid.clear();
	};

	void addForce(PxRigidDynamic* p, double t);
	void forceVertex(std::string tipo, Vector3D pos, Vector3D vel, float intensity = 1000.0f, float rad = 50.0f, float time = 1.5f);
	void clearForceVertex();
	void changeArea(Vector3D aMin, Vector3D aMax) { areaMin = aMin; areaMax = aMax; };
	void resetArea() {
		areaMin = Vector3D(-20, -20, -20);
		areaMax = Vector3D(100, 100, 100);
	}
};

