//
#include "Proyectil.h"
#include "Vector3D.h"


Proyectil::Proyectil(Vector3D pos, Vector3D dir, float velReal, float mReal, float velSim, float dumping, float grav) :
	Particle(pos, dir.Normalize()* velSim, Vector3D(0, 0, 0), dumping, masaSim(mReal, velReal, velSim), gravSim(grav, velReal, velSim)), vel(velSim)
{
	setA(Vector3D(0.0f, -grav, 0.0f));
};

float Proyectil::masaSim(float mReal, float velSim, float velReal)
{
	return (mReal * pow(velReal, 2)) / pow(velSim, 2);
}

float Proyectil::gravSim(float grav, float velSim, float velReal)
{
	return (grav * pow(velSim, 2)) / pow(velReal, 2);
}
