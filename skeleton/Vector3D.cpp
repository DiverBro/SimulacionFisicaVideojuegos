#include "Vector3D.h"

Vector3D::Vector3D(int x, int y, int z) : x(x), y(y), z(z)
{
}

Vector3D Vector3D::Normalize()
{
	int mod = sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
	return Vector3D(this->x / mod, this->y / mod, this->z / mod);
}

int Vector3D::scalarX(Vector3D seg)
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
