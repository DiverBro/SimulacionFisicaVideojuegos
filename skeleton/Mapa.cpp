#include "Mapa.h"
#include <iostream>
#include <algorithm>
using namespace physx;
Mapa::Mapa(int i, Vector3D startPos, Vector3D blockSize, PxRigidDynamic* ball, PxPhysics* gPhysics, PxScene* gScene) : startPos(startPos), blockSize(blockSize), i(i), proyectil(ball), gPhysics(gPhysics), gScene(gScene)
{
	partSys = new ParticleSystem();
	rigidSysExp = new RigidBodySystem();
	rigidSysWind = new RigidBodySystem();
	fillMaps("mapas.txt");
	createMap(map[i], startPos, blockSize);
}

void Mapa::update(double t, float angle)
{
	rigidSysWind->addForce(proyectil, t);
	rigidSysExp->addForce(proyectil, t);

	if (proyectil->getGlobalPose().p.x >= metaXBegin && proyectil->getGlobalPose().p.x <= metaXEnd
		&& proyectil->getGlobalPose().p.y >= metaYBegin && proyectil->getGlobalPose().p.y <= metaYEnd) {
		victoria = true;
	}
	if (posViento.getX() != -1)
		partSys->update(t, 3, posViento, true);

	if (victoria)
	{
		tiempo += t;
		partSys->update(t, 2, Vector3D(10, 0, -5), false);
		partSys->update(t, 2, Vector3D(35, 0, -5), false);
		if (tiempo >= 6)
		{
			changeMap(startPos, blockSize);
			victoria = false;
			tiempo = 0;
			partSys->clearParticles();
		}
	}

	if (arrow) {
		float angleRad = angle * PxPi / 180.0f;

		// Vector frente en el plano XY (rotando sobre Z)
		PxVec3 forward(cosf(angleRad), sinf(angleRad), 0.0f);

		float distancia = 2.5f;

		PxTransform newTransform(initialArrowPos + forward * distancia, PxQuat(angleRad, PxVec3(0.0f, 0.0f, 1.0f)));
		if (!proyectil->isSleeping()) {
			newTransform = PxTransform(Vector3(10000, 10000, 10000), PxQuat(angleRad, PxVec3(0.0f, 0.0f, 1.0f)));
		}

		if (arrow->transform) delete arrow->transform;
		arrow->transform = new PxTransform(newTransform);
	}
}

void Mapa::createMap(std::string filename, Vector3D startPos, Vector3D blockSize)
{
	std::ifstream file(filename);
	if (!file.is_open()) return;

	int width, height, velWindX, velWindY, expIntesnity, expRad;
	file >> width >> height >> velWindX >> velWindY >> expIntesnity >> expRad;

	std::string line;
	std::getline(file, line); // consume el salto de línea después del height

	for (int y = 0; y < height; ++y)
	{
		std::getline(file, line);
		for (int x = 0; x < width; ++x)
		{
			char c = line[x];
			Vector3D pos = startPos + Vector3D(x * blockSize.getX(), (height - 1 - y) * blockSize.getY(), 0);

			if (c == 'x')
			{

				PxRigidStatic* cube = gPhysics->createRigidStatic(PxTransform(pos.getX(), pos.getY(), pos.getZ()));
				PxShape* shape = CreateShape(PxBoxGeometry(blockSize.getX() * 0.5f, blockSize.getY() * 0.5f, blockSize.getZ() * 0.5f));
				cube->attachShape(*shape);
				//PxRigidBodyExt::updateMassAndInertia(*cube, 0);
				gScene->addActor(*cube);
				objetos.push_back(new RenderItem(shape, cube, Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
			}
			else if (c == 'm')
			{
				objetos.push_back(new RenderItem(
					CreateShape(PxBoxGeometry(blockSize.getX() * 0.5f, blockSize.getY() * 0.5f, blockSize.getZ() * 0.5f)),
					new PxTransform(pos.getX(), pos.getY(), pos.getZ()),
					Vector4(0.0f, 1.0f, 0.0f, 1.0f)
				));
				metaXBegin = pos.getX() - blockSize.getX() / 2; metaXEnd = pos.getX() + blockSize.getX() / 2;
				metaYBegin = pos.getY() - blockSize.getX() / 2; metaYEnd = pos.getY() + blockSize.getY() / 2;
			}
			else if (c == 'b') // otro tipo de bloque
			{
				proyectil->setGlobalPose(PxTransform(pos.getX(), pos.getY(), pos.getZ()));
				proyectil->setLinearVelocity(PxVec3(0, 0, 0));
				proyectil->putToSleep();
				posIniPr = pos;
				arrow = new RenderItem(CreateShape(PxBoxGeometry(0.5f, 0.2f, 0.2f)),
					new PxTransform(pos.getX(), pos.getY(), pos.getZ()), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
				initialArrowPos = arrow->transform->p;
				objetos.push_back(arrow);
			}
			else if (c == 'V')
			{
				Vector3D vel(velWindX, velWindY, 0);
				partSys->forceVertex("wind", Vector3D(0, 0, 0), vel);
				rigidSysWind->forceVertex("wind", Vector3D(0, 0, 0), vel);
				float longitud = blockSize.getX() * (width / 2);

				Vector3D dirArea = vel.Normalize();
				Vector3D p1 = pos;
				Vector3D p2 = pos + dirArea * longitud;

				p1.setY(p1.getY() - blockSize.getY() / 2);
				p2.setY(p2.getY() + blockSize.getY() / 2);
				p1.setZ(p1.getZ() - blockSize.getZ() / 2);
				p2.setZ(p2.getZ() + blockSize.getZ() / 2);

				float minX = (p1.getX() < p2.getX()) ? p1.getX() : p2.getX();
				float minY = (p1.getY() < p2.getY()) ? p1.getY() : p2.getY();
				float minZ = (p1.getZ() < p2.getZ()) ? p1.getZ() : p2.getZ();
				float maxX = (p1.getX() > p2.getX()) ? p1.getX() : p2.getX();
				float maxY = (p1.getY() > p2.getY()) ? p1.getY() : p2.getY();
				float maxZ = (p1.getZ() > p2.getZ()) ? p1.getZ() : p2.getZ();

				partSys->changeArea(Vector3D(minX, minY, minZ), Vector3D(maxX, maxY, maxZ));
				rigidSysWind->changeArea(Vector3D(minX, minY, minZ), Vector3D(maxX, maxY, maxZ));
				posViento = pos;
			}
			else if (c == 'E') {
				rigidSysExp->forceVertex("explosion", pos, Vector3D(0, 0, 0), expIntesnity, expRad, -1);
				rigidSysExp->resetArea();
				objetos.push_back(new RenderItem(
					CreateShape(PxSphereGeometry(expRad - 1)),
					new PxTransform(pos.getX(), pos.getY(), pos.getZ()),
					Vector4(1.0f, 1.0f, 0.0f, 0.1f)
				));
			}
		}
	}
}

void Mapa::changeMap(Vector3D startPos, Vector3D blockSize)
{
	posViento.setX(-1);
	rigidSysWind->clearForceVertex();
	rigidSysExp->clearForceVertex();
	partSys->clearForceVertex();
	for (RenderItem* o : objetos) {
		if (o->actor)
		{
			PxRigidActor* actor =
				const_cast<PxRigidActor*>(o->actor);

			if (actor->getScene())
				gScene->removeActor(*actor);

			actor->release();
		}

		o->release();
	}

	objetos.clear();
	i++;
	createMap(map[i], startPos, blockSize);
}

void Mapa::resetLevel(Vector3D startPos, Vector3D blockSize)
{
	posViento.setX(-1);
	rigidSysWind->clearForceVertex();
	rigidSysExp->clearForceVertex();
	partSys->clearForceVertex();
	for (RenderItem* o : objetos) {
		if (o->actor)
		{
			PxRigidActor* actor =
				const_cast<PxRigidActor*>(o->actor);

			if (actor->getScene())
				gScene->removeActor(*actor);

			actor->release();
		}

		o->release();
	}

	objetos.clear();
	createMap(map[i], startPos, blockSize);
}

void Mapa::fillMaps(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) return;

	int length;
	file >> length;
	std::string line;
	std::getline(file, line);
	for (int i = 0; i < length; i++) {
		std::getline(file, line);
		map.push_back(line);
	}
}
