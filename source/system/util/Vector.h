#pragma once
#include "../dx11/dx11mathutil.h"

struct Float3 : public DirectX::XMFLOAT3
{
	Float3() { this->x = this->y = this->z = 0; }
	Float3(float _f);
	Float3(float _x, float _y, float _z);
	Float3(const XMFLOAT3& _f3) { this->x = _f3.x; this->y = _f3.y; this->z = _f3.z; }

	Float3 operator+(const Float3& _f3);
	Float3 operator-(const Float3& _f3);
	Float3 operator*(const Float3& _f3);

	void operator*=(const Float3& _f3);
	void operator*=(float _f);
	void operator+=(const Float3& _f3);
	void operator+=(float _f);
	void operator-=(const Float3& _f3);

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

struct INT3 {
	int x, y, z;
	INT3() { this->x = this->y = this->z = 0; }
	INT3(int _i)
	{
		this->x = this->y = this->z = _i;
	}
	INT3(int _x, int _y, int _z)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
	}
	INT3(const INT3& _i3)
	{
		this->x = _i3.x;
		this->y = _i3.y;
		this->z = _i3.z;
	}
	INT3(const Float3& _f3)
	{
		this->x = static_cast<int>(_f3.x);
		this->y = static_cast<int>(_f3.y);
		this->z = static_cast<int>(_f3.z);
	}
	INT3 operator+(const INT3& _i3)
	{
		INT3 v3;
		v3.x = this->x + _i3.x;
		v3.y = this->y + _i3.y;
		v3.z = this->z + _i3.z;
		return v3;
	}
	INT3 operator-(const INT3& _i3)
	{
		INT3 v3;
		v3.x = this->x - _i3.x;
		v3.y = this->y - _i3.y;
		v3.z = this->z - _i3.z;
		return v3;
	}
};

struct INT2 {
	int x, y;
	INT2() { this->x = this->y = 0; }
	INT2(int _i)
	{
		this->x = this->y = _i;
	}
	INT2(int _x, int _y)
	{
		this->x = _x;
		this->y = _y;
	}
	INT2(const INT2& _i3)
	{
		this->x = _i3.x;
		this->y = _i3.y;
	}
	INT2(const Float3& _f3)
	{
		this->x = static_cast<int>(_f3.x);
		this->y = static_cast<int>(_f3.y);
	}
	INT2 operator+(const INT2& _i2)
	{
		INT2 i2;
		i2.x = this->x + _i2.x;
		i2.y = this->y + _i2.y;
		return i2;
	}
	INT2 operator-(const INT2& _i2)
	{
		INT2 i2;
		i2.x = this->x - _i2.x;
		i2.y = this->y - _i2.y;
		return i2;
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