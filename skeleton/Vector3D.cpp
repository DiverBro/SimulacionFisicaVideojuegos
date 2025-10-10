#include "Vector3D.h"

Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z)
{
}

Vector3D Vector3D::Normalize()
{
	double mod = sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
	return Vector3D(this->x / mod, this->y / mod, this->z / mod);
}

double Vector3D::scalarX(Vector3D seg)
{
	int xRes, yRes, zRes;
	xRes = this->x * seg.x;
	yRes = this->y * seg.y;
	zRes = this->z * seg.z;
	return xRes + yRes + zRes;
}

Vector3D Vector3D::vectorialX(Vector3D seg)
{
	int xRes, yRes, zRes;
	xRes = this->y * seg.z - this->z * seg.y;
	yRes = this->x * seg.z - this->z * seg.x;
	zRes = this->x * seg.y - this->y * seg.x;
	return Vector3D(xRes, yRes, zRes);
}
