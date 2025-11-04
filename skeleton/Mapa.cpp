#include "Mapa.h"
#include <iostream>
using namespace physx;
Mapa::Mapa(const std::string& filename, Vector3D startPos, Vector3D blockSize, Proyectil* pr) : proyectil(pr)
{
	partSys = new ParticleSystem();
	std::ifstream file(filename);
	if (!file.is_open()) return;

	int width, height;
	file >> width >> height;

	std::string line;
	std::getline(file, line); // consume el salto de línea después del height

	for (int y = 0; y < height; ++y)
	{
		std::getline(file, line);
		for (int x = 0; x < width; ++x)
		{
			char c = line[x];
			Vector3D pos = startPos + Vector3D(x * blockSize.getX(), (height - 1 - y) * blockSize.getY(), 0);

			if (c == 'x') // bloque sólido
				objetos.push_back(new RenderItem(
					CreateShape(PxBoxGeometry(blockSize.getX() * 0.5f, blockSize.getY() * 0.5f, blockSize.getZ() * 0.5f)),
					new PxTransform(pos.getX(), pos.getY(), pos.getZ()),
					Vector4(1.0f, 0.0f, 0.0f, 1.0f)
				));
			else if (c == 'm') // bloque sólido
			{
				objetos.push_back(new RenderItem(
					CreateShape(PxBoxGeometry(blockSize.getX() * 0.5f, blockSize.getY() * 0.5f, blockSize.getZ() * 0.5f)),
					new PxTransform(pos.getX(), pos.getY(), pos.getZ()),
					Vector4(0.0f, 1.0f, 0.0f, 1.0f)
				));
				metaXBegin = pos.getX(); metaXEnd = pos.getX() + blockSize.getX();
				metaYBegin = pos.getY(); metaYEnd = pos.getY() + blockSize.getY();
			}
			else if (c == 'b') // otro tipo de bloque
			{
				pr->setPose(PxVec3(pos.getX(), pos.getY(), pos.getZ()));
				posIniPr = pos;
			}
			else if (c == 'V') // otro tipo de bloque
			{
				Vector3D vel = Vector3D(65, 0, 0);
				partSys->forceVertex(new ForceGenerator(vel, 1, 0));
				if (vel.getX() < 0)
					partSys->changeArea(Vector3D(pos.getX() - blockSize.getX() * width, pos.getY() - blockSize.getY(), pos.getZ() - blockSize.getZ()),
						Vector3D(pos.getX(), pos.getY() + blockSize.getY(), pos.getZ() + blockSize.getZ()));
				else
					partSys->changeArea(Vector3D(pos.getX(), pos.getY() - blockSize.getY(), pos.getZ() - blockSize.getZ()), Vector3D(pos.getX() + blockSize.getX() * width, pos.getY() + blockSize.getY(), pos.getZ() + blockSize.getZ()));
			}
		}
	}

	for (RenderItem* rI : objetos)
		RegisterRenderItem(rI);
}

void Mapa::update(double t)
{
	if (proyectil->getPose().x >= metaXBegin && proyectil->getPose().x <= metaXEnd
		&& proyectil->getPose().y >= metaYBegin && proyectil->getPose().y <= metaYEnd) {
		proyectil->setVel(Vector3D(0, 0, 0));
		proyectil->setPose(PxVec3(posIniPr.getX(), posIniPr.getY(), posIniPr.getZ()));
		victoria = true;
	}
	if (victoria)
	{
		tiempo += t;
		partSys->update(t, 2, Vector3D(10, 0, -5));
		partSys->update(t, 2, Vector3D(35, 0, -5));
		if (tiempo >= 6)
		{
			victoria = false;
			tiempo = 0;
			partSys->clearParticles();
		}
	}
	partSys->addForce(proyectil, t);
}
