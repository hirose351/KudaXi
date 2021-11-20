#include	"vector.h"
#include	<DirectXMath.h>
#include	<cmath>

Vector3::Vector3(float _x, float _y, float _z)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
}

Vector3 Vector3::operator+(const Vector3& _v3)
{
	Vector3 work;
	work.x = this->x + _v3.x;
	work.y = this->y + _v3.y;
	work.z = this->z + _v3.z;
	return work;
}

Vector3 Vector3::operator-(const Vector3& _v3)
{
	Vector3 work;
	work.x = this->x - _v3.x;
	work.y = this->y - _v3.y;
	work.z = this->z - _v3.z;
	return work;
}

Vector3 Vector3::operator*(const Vector3& _v3)
{
	Vector3 work;
	work.x = this->x * _v3.x;
	work.y = this->y * _v3.y;
	work.z = this->z * _v3.z;
	return work;
}

void Vector3::operator*=(const Vector3& _v3)
{
	this->x *= _v3.x;
	this->y *= _v3.y;
	this->z *= _v3.z;
}

void Vector3::operator*=(float _f)
{
	this->x *= _f;
	this->y *= _f;
	this->z *= _f;
}

void Vector3::operator+=(const Vector3& _v3)
{
	this->x += _v3.x;
	this->y += _v3.y;
	this->z += _v3.z;
}

void Vector3::operator+=(float _f)
{
	this->x += _f;
	this->y += _f;
	this->z += _f;
}

void Vector3::operator-=(const Vector3& _v3)
{
	this->x -= _v3.x;
	this->y -= _v3.y;
	this->z -= _v3.z;
}

Vector3 Vector3::operator+(const XMFLOAT3& _f3)
{
	Vector3 work;
	work.x = this->x + _f3.x;
	work.y = this->y + _f3.y;
	work.z = this->z + _f3.z;
	return work;
}

Vector3 Vector3::operator-(const XMFLOAT3& _f3)
{
	Vector3 work;
	work.x = this->x - _f3.x;
	work.y = this->y - _f3.y;
	work.z = this->z - _f3.z;
	return work;
}

Vector3 Vector3::operator*(const XMFLOAT3& _f3)
{
	Vector3 work;
	work.x = this->x * _f3.x;
	work.y = this->y * _f3.y;
	work.z = this->z * _f3.z;
	return work;
}

Vector3 Vector3::operator*(float _f)
{
	Vector3 work;
	work.x = this->x * _f;
	work.y = this->y * _f;
	work.z = this->z * _f;
	return work;
}

void Vector3::operator*=(const XMFLOAT3 &_f3)
{
	this->x *= _f3.x;
	this->y *= _f3.y;
	this->z *= _f3.z;
}

void Vector3::operator+=(const XMFLOAT3 &_f3)
{
	this->x += _f3.x;
	this->y += _f3.y;
	this->z += _f3.z;
}

void Vector3::operator-=(const XMFLOAT3 &_f3)
{
	this->x -= _f3.x;
	this->y -= _f3.y;
	this->z -= _f3.z;
}

void Vector3::operator=(const XMFLOAT3 &_f3)
{
	this->x = _f3.x;
	this->y = _f3.y;
	this->z = _f3.z;
}

bool Vector3::operator!=(const Vector3 &_v3)
{
	if ((this->x) == _v3.x)
		return false;
	if ((this->y) == _v3.y)
		return false;
	if ((this->z) == _v3.z)
		return false;

	return true;
}

bool Vector3::operator==(const Vector3& _v3)
{
	if ((this->x) != _v3.x)
		return false;
	if ((this->y) != _v3.y)
		return false;
	if ((this->z) != _v3.z)
		return false;

	return true;
}

DirectX::XMFLOAT3 Vector3::ConversionToXMFLOAT3()
{
	return DirectX::XMFLOAT3(this->x, this->y, this->z);
}

