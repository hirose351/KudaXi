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

public:
	// Todo:âÒì]Ç∆Ç©à⁄ìÆÇ∆Ç©ÇÃä÷êîçÏÇÈ


	void SetPosition(const Vector3& _pos) { mPosition = _pos; }
	void SetRotation(const Vector3& _rot) { mRotation = _rot; }
	void SetScale(const Vector3& _sc) { mScale = _sc; }
	void SetAngle(const Vector3& _ang) { mAngle = _ang; }
	void SetMtx(const DirectX::XMFLOAT4X4 _mtx) { mMtx = _mtx; }
	Vector3 GetPosition() { return mPosition; }
	Vector3 GetRotation() { return mRotation; }
	Vector3 GetScale() { return mScale; }
	Vector3 GetAngle() { return mAngle; }
	DirectX::XMFLOAT4X4 GetMtx() { return mMtx; }
};