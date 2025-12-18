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
	if (proyectil->getGlobalPose().p.x >= metaXBegin && proyectil->getGlobalPose().p.x <= metaXEnd
		&& proyectil->getGlobalPose().p.y >= metaYBegin && proyectil->getGlobalPose().p.y <= metaYEnd) {
		proyectil->setGlobalPose(PxTransform({ 1000, 1000, 0 }));
		victoria = true;
	}
	if (posViento.getX() != -1)
		partSys->update(t, 3, posViento, true);
	rigidSysWind->addForce(proyectil, t);
	rigidSysExp->addForce(proyectil, t);

	if (victoria)
	{
		tiempo += t;
		partSys->update(t, 2, Vector3D(10, 0, -5), false);
		partSys->update(t, 2, Vector3D(35, 0, -5), false);
		if (tiempo >= 6)
		{
			changeMap();
			victoria = false;
			tiempo = 0;
			partSys->clearParticles();
		}
	}

	float angleRad = angle * PxPi / 180.0f;
	PxVec3 forward(cosf(angleRad), sinf(angleRad), 0.0f);

	float spacing = blockSize.getX() * 0.04f;

	for (int i = 0; i < powerBar.size(); ++i) {
		RenderItem* bar = powerBar[i];

		float dist = spacing * (i + 1);

		PxVec3 pos = PxVec3(
			initialArrowPos.x,
			initialArrowPos.y,
			initialArrowPos.z
		) + forward * (dist + blockSize.getX() * 0.25);

		PxTransform t(
			pos,
			PxQuat(angleRad, PxVec3(0, 0, 1))
		);

		delete bar->transform;
		bar->transform = new PxTransform(t);
	}

	if (!proyectil->isSleeping())
	{
		for (RenderItem* bar : powerBar)
		{
			if (bar->transform)
				delete bar->transform;

			bar->transform = new PxTransform(PxVec3(10000.0f, 10000.0f, 10000.0f));
		}
		reset = false;
	}

	if (proyectil->isSleeping() && !victoria && !reset) {
		resetLevel();
		reset = true;
	}
}

void Mapa::createMap(std::string filename, Vector3D startPos, Vector3D blockSize)
{
	if (filename == "win.txt") startPos = Vector3D(-5, 0, -5);
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
				objetos.push_back(new RenderItem(shape, cube, Vector4(1.0f, 0.0f, 1.0f, 1.0f)));
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
				initialArrowPos = PxVec3(pos.getX(), pos.getY(), pos.getZ());
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

void Mapa::changeMap()
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

void Mapa::resetLevel()
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

void Mapa::chargeBar(float p)
{
	for (RenderItem* o : powerBar) {
		o->release();
	}
	powerBar.clear();

	float tam = 80;

	for (int i = 1; i <= p; i++) {
		float rg = i / tam;
		powerBar.push_back(new RenderItem(
			CreateShape(PxBoxGeometry(blockSize.getX() * 0.02f, blockSize.getY() * 0.05f, blockSize.getZ() * 0.05f)),
			new PxTransform(startPos.getX() + blockSize.getX() * 0.04 * i, startPos.getY(), blockSize.getZ() / 2),
			Vector4(rg, 1 - rg, 0.0f, 1.0f)
		));
	}
}
