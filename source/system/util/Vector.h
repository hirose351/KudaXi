#pragma once
#include "../dx11/dx11mathutil.h"

struct Float3 : public DirectX::XMFLOAT3
{
	Float3() {}
	Float3(float _x, float _y, float _z);
	Float3(const XMFLOAT3& _f3) { this->x = _f3.x; this->y = _f3.y; this->z = _f3.z; }

	Float3 operator+(const Float3& _v3);
	Float3 operator-(const Float3& _v3);
	Float3 operator*(const Float3& _v3);

	void operator*=(const Float3& _v3);
	void operator*=(float _f);
	void operator+=(const Float3& _v3);
	void operator+=(float _f);
	void operator-=(const Float3& _v3);

	Float3 operator+(const XMFLOAT3& _f3);
	Float3 operator-(const XMFLOAT3& _f3);
	Float3 operator*(const XMFLOAT3& _f3);

	Float3 operator+(float _f);
	Float3 operator-(float _f);
	Float3 operator*(float _f);
	Float3 operator/(float _f);

	void operator*=(const XMFLOAT3& _f3);
	void operator+=(const XMFLOAT3& _f3);
	void operator-=(const XMFLOAT3& _f3);
	void operator= (const XMFLOAT3& _f3);

	bool operator==(const Float3& _v3);
	bool operator!=(const Float3& _v3);

	XMFLOAT3 ConversionToXMFLOAT3();


	float dot(const Float3 &r) const {
		return x * r.x + y * r.y + z * r.z;
	}

	Float3 cross(const Float3 &r) const {
		return Float3(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x);
	}

	float length() const {
		return sqrtf(lengthSq());
	}

	float lengthSq() const {
		return x * x + y * y + z * z;
	}

	void norm() {
		const float len = length();
		if (len > 0.0f)
		{
			x /= len;
			y /= len;
			z /= len;
		}
	}

	Float3 getNorm() const {
		const float len = length();
		if (len > 0.0f)
		{
			return Float3(x / len, y / len, z / len);
		}
		return Float3(0.0f, 0.0f, 0.0f);
	}
};

struct FLOAT4X4 :public DirectX::XMFLOAT4X4 {

	FLOAT4X4 operator=(float a) {
		_11 = _12 = _13 = _14 =
			_21 = _22 = _23 = _24 =
			_31 = _32 = _33 = _34 =
			_41 = _42 = _43 = _44 = a;
	}
};