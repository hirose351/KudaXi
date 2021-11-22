#pragma once
#include	"../../system/util/vector.h"

class Transform
{
protected:
	Vector3 mPosition;
	Vector3 mMove;
	Vector3 mRotation;
	Vector3 mScale;
	Vector3 mAngle;
	DirectX::XMFLOAT4X4 mMtx;
	DirectX::XMFLOAT4X4 mLocalMtx;
	DirectX::XMFLOAT4X4 mScaleMtx;

public:
	Transform();

	// Todo:âÒì]Ç∆Ç©à⁄ìÆÇ∆Ç©ÇÃä÷êîçÏÇÈ

	void AddPosition();
	void CreateScaleMtx();

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