#pragma once
#include <math.h>
#include "../dx11/util/dx11mathutil.h"

struct Vector3
{
public:

	float x, y, z;
	Vector3() {};
	Vector3(float val)
	{
		(this->x) = val;
		(this->y) = val;
		(this->z) = val;
	}
	Vector3(float x, float y, float z)
	{
		(this->x) = x;
		(this->y) = y;
		(this->z) = z;
	};

	operator XMFLOAT3() const
	{
		return XMFLOAT3(this->x, this->y, this->z);
	}

	Vector3 operator + (const Vector3 dvec) const
	{
		Vector3 v3;

		v3.x = ((this)->x) + dvec.x;
		v3.y = ((this)->y) + dvec.y;
		v3.z = ((this)->z) + dvec.z;
		return v3;
	};

	Vector3 operator - (const Vector3 dvec) const
	{
		Vector3 v3;

		v3.x = ((this)->x) - dvec.x;
		v3.y = ((this)->y) - dvec.y;
		v3.z = ((this)->z) - dvec.z;
		return v3;
	};

	Vector3 operator * (float fDat) const
	{
		Vector3 v3;

		v3.x = ((this)->x) * fDat;
		v3.y = ((this)->y) * fDat;
		v3.z = ((this)->z) * fDat;

		return v3;
	};

	Vector3 operator / (float fDat) const
	{
		Vector3 v3;

		if (fDat == 0.0f)
		{
			return *this;
		}

		v3.x = ((this)->x) / fDat;
		v3.y = ((this)->y) / fDat;
		v3.z = ((this)->z) / fDat;
		return v3;
	};

	Vector3& operator += (const Vector3 dvec)
	{
		((this)->x) += dvec.x;
		((this)->y) += dvec.y;
		((this)->z) += dvec.z;
		return *this;
	};

	Vector3& operator -= (const Vector3 dvec)
	{
		((this)->x) -= dvec.x;
		((this)->y) -= dvec.y;
		((this)->z) -= dvec.z;
		return *this;
	};

	Vector3& operator *= (float fDat)
	{
		((this)->x) *= fDat;
		((this)->y) *= fDat;
		((this)->z) *= fDat;
		return *this;
	};

	Vector3& operator /= (float fDat)
	{

		if (fDat == 0.0f)
		{
			return *this;
		}

		((this)->x) /= fDat;
		((this)->y) /= fDat;
		((this)->z) /= fDat;
		return *this;
	};

	bool operator == (const Vector3 dvec) const
	{
		if ((this->x) != dvec.x)
			return false;
		if ((this->y) != dvec.y)
			return false;
		if ((this->z
			) != dvec.z)
			return false;

		return true;
	}

	bool operator != (const Vector3 dvec) const
	{
		if ((this->x) == dvec.x)
			return false;
		if ((this->y) == dvec.y)
			return false;
		if ((this->z) == dvec.z)
			return false;

		return true;
	}


	Vector3 Normalize()
	{
		double len;
		double x, y, z;
		Vector3 out;

		x = (double)(this->x);//精度保持のため一度doubleでやる
		y = (double)(this->y);
		z = (double)(this->z);

		//割と死すべきコード
		len = sqrt(x * x + y * y + z * z);

		if (len < (1e-6)) return Vector3(0);

		len = 1.0 / len;
		x *= len;
		y *= len;
		z *= len;

		out.x = (float)x;
		out.y = (float)y;
		out.z = (float)z;

		return out;
	}

	Vector3 Direction(Vector3 v2)//自分から引数に向いたベクトルを返す
	{
		Vector3 direction;
		direction = v2 - (*this);
		//direction.x = v2.x - (this->x);
		//direction.y = v2.y - (this->y);
		direction.Normalize();
		return direction;
	}

	float Dot(Vector3 dvec)
	{
		return ((this->x)*(dvec.x) + (this->y)*(dvec.y) + (this->z)*(dvec.z));
	}

	Vector3 Cross(Vector3 dvec)
	{
		Vector3 vec;
		double x1, y1, z1, x2, y2, z2;

		x1 = (double)(this->x);
		y1 = (double)(this->y);
		z1 = (double)(this->z);
		x2 = (double)(dvec.x);
		y2 = (double)(dvec.y);
		z2 = (double)(dvec.z);

		vec.x = (float)(y1 * z2 - z1 * y2);
		vec.y = (float)(z1 * x2 - x1 * z2);
		vec.z = (float)(x1 * y2 - y1 * x2);

		return vec;
	}
};

struct Vector2
{
	float x, y;
	Vector2() {};
	Vector2(float val)
	{
		(this->x) = val;
		(this->y) = val;
	}
	Vector2(float x, float y)
	{
		(this->x) = x;
		(this->y) = y;
	};

	Vector2 operator + (const Vector2 dvec) const
	{
		Vector2 v2;

		v2.x = ((this)->x) + dvec.x;
		v2.y = ((this)->y) + dvec.y;
		return v2;
	};

	Vector2 operator - (const Vector2 dvec) const
	{
		Vector2 v2;

		v2.x = ((this)->x) - dvec.x;
		v2.y = ((this)->y) - dvec.y;
		return v2;
	};

	Vector2 operator * (float fDat) const
	{
		Vector2 v2;

		v2.x = ((this)->x) * fDat;
		v2.y = ((this)->y) * fDat;
		return v2;
	};

	Vector2 operator / (float fDat) const
	{
		Vector2 v2;

		if (fDat == 0.0f)
		{
			return *this;
		}

		v2.x = ((this)->x) / fDat;
		v2.y = ((this)->y) / fDat;
		return v2;
	};

	Vector2& operator += (const Vector2 dvec)
	{
		((this)->x) += dvec.x;
		((this)->y) += dvec.y;
		return *this;
	};

	Vector2& operator -= (const Vector2 dvec)
	{
		((this)->x) -= dvec.x;
		((this)->y) -= dvec.y;
		return *this;
	};

	Vector2& operator *= (float fDat)
	{
		((this)->x) *= fDat;
		((this)->y) *= fDat;
		return *this;
	};

	Vector2& operator /= (float fDat)
	{

		if (fDat == 0.0f)
		{
			return *this;
		}

		((this)->x) /= fDat;
		((this)->y) /= fDat;
		return *this;
	};

	bool operator == (const Vector2 dvec) const
	{
		if ((this->x) != dvec.x)
			return false;
		if ((this->y) != dvec.y)
			return false;

		return true;
	}

	bool operator != (const Vector2 dvec) const
	{
		if ((this->x) == dvec.x)
			return false;
		if ((this->y) == dvec.y)
			return false;

		return true;
	}

	Vector2 Normalize()
	{
		double len;
		double x, y;
		Vector2 out;

		x = (double)(this->x);//精度保持のため一度doubleでやる
		y = (double)(this->y);

		//割と死すべきコード
		len = sqrt(x * x + y * y);

		if (len < (1e-6)) return 0;

		len = 1.0 / len;
		x *= len;
		y *= len;

		out.x = (float)x;
		out.y = (float)y;

		return out;
	}

	Vector2 Direction(Vector2 v2)//自分から引数に向いたベクトルを返す
	{
		Vector2 direction;
		direction = v2 - (*this);
		//direction.x = v2.x - (this->x);
		//direction.y = v2.y - (this->y);
		direction.Normalize();
		return direction;
	}

	float Dot(Vector2 dvec)
	{
		return ((this->x)*(dvec.x) + (this->y)*(dvec.y));
	}
};