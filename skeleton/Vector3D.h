#pragma once
#include <algorithm>
class Vector3D
{
private:
	int x, y, z;
public:
	Vector3D(int x, int y, int z);
	Vector3D Normalize();
	int scalarX(Vector3D seg);
	Vector3D vectorialX(Vector3D seg);
};

