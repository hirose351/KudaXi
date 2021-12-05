#pragma once

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
	INT3(const INT3& _i3) { this->x = _i3.x; this->y = _i3.y; this->z = _i3.z; }
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

