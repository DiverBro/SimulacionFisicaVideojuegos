#include "Proyectil.h"
#include "Vector3D.h"
#include <iostream>


Proyectil::Proyectil(Vector3D pos, Vector3D dir, Vector3D ace, float velReal, float mReal, float velSim, Vector4 col, float dumping, Vector3D grav) :
	Particle(pos, dir.Normalize()* velSim, ace, dumping, masaSim(mReal, velSim, velReal), gravSim(grav.getY(), velSim, velReal), col)
{
	setAce(Vector3D(ace.getX(), ace.getY() - gravSim(grav.getY(), velSim, velReal), ace.getZ()));
};

float Proyectil::masaSim(float mReal, float velSim, float velReal)
{
	return (mReal * pow(velReal, 2)) / pow(velSim, 2);
}

float Proyectil::gravSim(float grav, float velSim, float velReal)
{
	return (grav * pow(velSim, 2)) / pow(velReal, 2);
}
