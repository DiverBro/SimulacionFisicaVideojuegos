#pragma once
#include <algorithm>
#include <PxPhysicsAPI.h>

class Vector3D
{
private:
	double x, y, z;
public:
	Vector3D(double x, double y, double z);
	Vector3D(physx::PxVec3 v);
	Vector3D Normalize();
	double scalarX(Vector3D seg);
	Vector3D vectorialX(Vector3D seg);
	double getX() {
		return x;
	};
	double getY() {
		return y;
	};
	double getZ() {
		return z;
	};

	Vector3D operator*(double n) {
		return Vector3D(this->x * n, this->y * n, this->z * n);
	}

	Vector3D operator/(double n) {
		return Vector3D(this->x / n, this->y / n, this->z / n);
	}

	Vector3D operator*(Vector3D n) {
		return Vector3D(this->x * n.getX(), this->y * n.getY(), this->z * n.getZ());
	}

	Vector3D operator/(Vector3D n) {
		return Vector3D(this->x / n.getX(), this->y / n.getY(), this->z / n.getZ());
	}

	Vector3D operator+(Vector3D n) {
		return Vector3D(this->x + n.getX(), this->y + n.getY(), this->z + n.getZ());
	}

	Vector3D operator-(Vector3D n) {
		return Vector3D(this->x - n.getX(), this->y - n.getY(), this->z - n.getZ());
	}
};

