#include	"vector.h"
#include	<DirectXMath.h>

Float3::Float3(float _f)
{
	this->x = this->y = this->z = _f;
}

Float3::Float3(float _x, float _y, float _z)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
}

Float3 Float3::operator+(const Float3& _v3)
{
	Float3 v3;
	v3.x = this->x + _v3.x;
	v3.y = this->y + _v3.y;
	v3.z = this->z + _v3.z;
	return v3;
}

Float3 Float3::operator-(const Float3& _v3)
{
	Float3 v3;
	v3.x = this->x - _v3.x;
	v3.y = this->y - _v3.y;
	v3.z = this->z - _v3.z;
	return v3;
}

Float3 Float3::operator*(const Float3& _v3)
{
	Float3 v3;
	v3.x = this->x * _v3.x;
	v3.y = this->y * _v3.y;
	v3.z = this->z * _v3.z;
	return v3;
}

void Float3::operator*=(const Float3& _v3)
{
	this->x *= _v3.x;
	this->y *= _v3.y;
	this->z *= _v3.z;
}

void Float3::operator*=(float _f)
{
	this->x *= _f;
	this->y *= _f;
	this->z *= _f;
}

void Float3::operator+=(const Float3& _v3)
{
	this->x += _v3.x;
	this->y += _v3.y;
	this->z += _v3.z;
}

void Float3::operator+=(float _f)
{
	this->x += _f;
	this->y += _f;
	this->z += _f;
}

void Float3::operator-=(const Float3& _v3)
{
	this->x -= _v3.x;
	this->y -= _v3.y;
	this->z -= _v3.z;
}

Float3 Float3::operator+(const XMFLOAT3& _f3)
{
	Float3 work;
	work.x = this->x + _f3.x;
	work.y = this->y + _f3.y;
	work.z = this->z + _f3.z;
	return work;
}

Float3 Float3::operator-(const XMFLOAT3& _f3)
{
	Float3 work;
	work.x = this->x - _f3.x;
	work.y = this->y - _f3.y;
	work.z = this->z - _f3.z;
	return work;
}

Float3 Float3::operator*(const XMFLOAT3& _f3)
{
	Float3 v3;
	v3.x = this->x * _f3.x;
	v3.y = this->y * _f3.y;
	v3.z = this->z * _f3.z;
	return v3;
}

Float3 Float3::operator+(float _f)
{
	Float3 v3;
	v3.x = this->x + _f;
	v3.y = this->y + _f;
	v3.z = this->z + _f;
	return v3;
}

Float3 Float3::operator-(float _f)
{
	Float3 v3;
	v3.x = this->x - _f;
	v3.y = this->y - _f;
	v3.z = this->z - _f;
	return v3;
}

Float3 Float3::operator*(float _f)
{
	Float3 v3;
	v3.x = this->x * _f;
	v3.y = this->y * _f;
	v3.z = this->z * _f;
	return v3;
}

Float3 Float3::operator/(float _f)
{
	Float3 v3;
	v3.x = this->x / _f;
	v3.y = this->y / _f;
	v3.z = this->z / _f;
	return v3;
}

void Float3::operator*=(const XMFLOAT3 &_f3)
{
	this->x *= _f3.x;
	this->y *= _f3.y;
	this->z *= _f3.z;
}

void Float3::operator+=(const XMFLOAT3 &_f3)
{
	this->x += _f3.x;
	this->y += _f3.y;
	this->z += _f3.z;
}

void Float3::operator-=(const XMFLOAT3 &_f3)
{
	this->x -= _f3.x;
	this->y -= _f3.y;
	this->z -= _f3.z;
}

void Float3::operator=(const XMFLOAT3 &_f3)
{
	this->x = _f3.x;
	this->y = _f3.y;
	this->z = _f3.z;
}

bool Float3::operator!=(const Float3 &_v3)
{
	if ((this->x) == _v3.x)
		return false;
	if ((this->y) == _v3.y)
		return false;
	if ((this->z) == _v3.z)
		return false;

	return true;
}

bool Float3::operator==(const Float3& _v3)
{
	if ((this->x) != _v3.x)
		return false;
	if ((this->y) != _v3.y)
		return false;
	if ((this->z) != _v3.z)
		return false;

	return true;
}

DirectX::XMFLOAT3 Float3::ConversionToXMFLOAT3()
{
	return DirectX::XMFLOAT3(this->x, this->y, this->z);
}

