#include "Proyectil.h"
#include "Vector3D.h"
#include <iostream>


Proyectil::Proyectil(Vector3D pos, Vector3D dir, Vector3D ace, float velReal, float mReal, float velSim, Vector4 col, float dumping, Vector3D grav) :
	Particle(pos, dir.Normalize()* velSim, ace, dumping, 1.0f, 3.0f, masaSim(mReal, velSim, velReal), gravSim(grav.getY(), velSim, velReal), col)
{
};

float Proyectil::masaSim(float mReal, float velSim, float velReal)
{
	return (mReal * pow(velReal, 2)) / pow(velSim, 2);
}
//ESCALADO DE MASA Y DE GRAVEDAD
Vector3D Proyectil::gravSim(float grav, float velSim, float velReal)
{
	return Vector3D(0, (grav * pow(velSim, 2)) / pow(velReal, 2), 0);
}
