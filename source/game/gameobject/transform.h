#pragma once
#include	"../../system/util/vector.h"

class Transform
{
protected:
	Vector3 mPosition;
	Vector3 mRotation;
	Vector3 mScale;
	Vector3 mAngle;
	DirectX::XMFLOAT4X4 mMtx;
};