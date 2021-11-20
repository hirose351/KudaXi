#pragma once
#include "../dx11/util/dx11mathutil.h"

struct Vector3 : public DirectX::XMFLOAT3
{
public:
	Vector3() {}
	Vector3(float _x, float _y, float _z);
	Vector3(const XMFLOAT3& _f3) { this->x = _f3.x; this->y = _f3.y; this->z = _f3.z; }

	Vector3 operator+(const Vector3& _v3);
	Vector3 operator-(const Vector3& _v3);
	Vector3 operator*(const Vector3& _v3);

	void operator*=(const Vector3& _v3);
	void operator*=(float _f);
	void operator+=(const Vector3& _v3);
	void operator+=(float _f);
	void operator-=(const Vector3& _v3);

	Vector3 operator+(const XMFLOAT3& _f3);
	Vector3 operator-(const XMFLOAT3& _f3);
	Vector3 operator*(const XMFLOAT3& _f3);
	Vector3 operator*(float _f);

	void operator*=(const XMFLOAT3& _f3);
	void operator+=(const XMFLOAT3& _f3);
	void operator-=(const XMFLOAT3& _f3);
	void operator= (const XMFLOAT3& _f3);

	bool operator==(const Vector3& _v3);
	bool operator!=(const Vector3& _v3);

	DirectX::XMFLOAT3 ConversionToXMFLOAT3();
};