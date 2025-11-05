#include "Mapa.h"
#include <iostream>
#include <algorithm>
using namespace physx;
Mapa::Mapa(const std::string& filename, Vector3D startPos, Vector3D blockSize, Proyectil* pr) : proyectil(pr)
{
	partSys = new ParticleSystem();
	std::ifstream file(filename);
	if (!file.is_open()) return;

	int width, height, velWindX, velWindY;
	file >> width >> height >> velWindX >> velWindY;

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
				objetos.push_back(new RenderItem(
					CreateShape(PxBoxGeometry(blockSize.getX() * 0.5f, blockSize.getY() * 0.5f, blockSize.getZ() * 0.5f)),
					new PxTransform(pos.getX(), pos.getY(), pos.getZ()),
					Vector4(1.0f, 0.0f, 0.0f, 1.0f)
				));
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
				pr->setPose(PxVec3(pos.getX(), pos.getY(), pos.getZ()));
				posIniPr = pos;
				arrow = new RenderItem(CreateShape(PxBoxGeometry(0.5f, 0.2f, 0.2f)),
					new PxTransform(pos.getX(), pos.getY(), pos.getZ()), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
				initialArrowPos = arrow->transform->p;
			}
			else if (c == 'V')
			{
				PxBoxGeometry geom(blockSize.getX() * (width / 2) * 0.5f, blockSize.getY() * 0.5f, blockSize.getZ() * 0.5f);

				Vector3D vel(velWindX, velWindY, 0);
				partSys->forceVertex(new ForceGenerator(vel, 1, 0));

				Vector3D dir = vel;
				float len = sqrt(dir.getX() * dir.getX() + dir.getY() * dir.getY());
				if (len > 0.001f)
					dir = Vector3D(dir.getX() / len, dir.getY() / len, 0);
				else
					dir = Vector3D(1, 0, 0);

				PxVec3 from(1, 0, 0);
				PxVec3 to(dir.getX(), dir.getY(), 0);
				PxQuat rot = PxShortestRotation(from, to);

				float longitud = blockSize.getX() * (width / 2);
				PxVec3 centro(pos.getX(), pos.getY(), pos.getZ());

				const float tol = 0.01f;

				// Ajuste del centro según dirección y signo
				if (fabs(dir.getY()) < tol) // horizontal
				{
					if (velWindX > 0) centro += PxVec3(dir.getX() * (longitud * 0.5f) - blockSize.getX() * 0.5f, 0, 0);
					else centro += PxVec3(dir.getX() * (longitud * 0.5f) + blockSize.getX() * 0.5f, 0, 0);
				}
				else if (fabs(dir.getX()) < tol) // vertical
				{
					if (velWindY > 0) centro += PxVec3(0, dir.getY() * (longitud * 0.5f) - blockSize.getY() * 0.5f, 0);
					else centro += PxVec3(0, dir.getY() * (longitud * 0.5f) + blockSize.getY() * 0.5f, 0);
				}
				else // diagonal
				{
					if (velWindX > 0 && velWindY < 0) centro += PxVec3(dir.getX() * (longitud * 0.5f) - blockSize.getX() * 0.5f, dir.getY() * (longitud * 0.5f) + blockSize.getY() * 0.5f, 0); // derecha-abajo
					else if (velWindX < 0 && velWindY < 0) centro += PxVec3(dir.getX() * (longitud * 0.5f) + blockSize.getX() * 0.5f, dir.getY() * (longitud * 0.5f) + blockSize.getY() * 0.5f, 0); // izquierda-abajo
					else if (velWindX > 0 && velWindY > 0) centro += PxVec3(dir.getX() * (longitud * 0.5f) - blockSize.getX() * 0.5f, dir.getY() * (longitud * 0.5f) - blockSize.getY() * 0.5f, 0); // derecha-arriba
					else if (velWindX < 0 && velWindY > 0) centro += PxVec3(dir.getX() * (longitud * 0.5f) + blockSize.getX() * 0.5f, dir.getY() * (longitud * 0.5f) - blockSize.getY() * 0.5f, 0); // izquierda-arriba
				}

				objetos.push_back(new RenderItem(
					CreateShape(geom),
					new PxTransform(centro, rot),
					Vector4(0.0f, 0.0f, 1.0f, 0.5f)
				));

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
			}
		}
	}
}

void Mapa::update(double t, float angle)
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
		partSys->update(t, 2, Vector3D(10, 0, -5), false);
		partSys->update(t, 2, Vector3D(35, 0, -5), false);
		if (tiempo >= 6)
		{
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

		PxTransform newTransform(initialArrowPos + forward * distancia, PxQuat(angleRad, PxVec3(0.0f, 0.0f, 1.0f))   );

		if (arrow->transform) delete arrow->transform;
		arrow->transform = new PxTransform(newTransform);
	}
	partSys->addForce(proyectil, t);
}
