#pragma once
#include	"../../system/util/vector.h"

class Transform
{
protected:
	Float3 mPosition;
	Float3 mMove;
	Float3 mRotation;
	Float3 mScale;
	Float3 mAngle;
	DirectX::XMFLOAT4X4 mMtx;
	DirectX::XMFLOAT4X4 mLocalMtx;
	DirectX::XMFLOAT4X4 mScaleMtx;

public:
	Transform();

	// Todo:âÒì]Ç∆Ç©à⁄ìÆÇ∆Ç©ÇÃä÷êîçÏÇÈ

	void AddPosition();
	void CreateScaleMtx();

	void SetPosition(const Float3& _pos) { mPosition = _pos; }
	void SetRotation(const Float3& _rot) { mRotation = _rot; }
	void SetScale(const Float3& _sc) { mScale = _sc; }
	void SetAngle(const Float3& _ang) { mAngle = _ang; }
	void SetMtx(const DirectX::XMFLOAT4X4 _mtx) { mMtx = _mtx; }
	Float3 GetPosition() { return mPosition; }
	Float3 GetRotation() { return mRotation; }
	Float3 GetScale() { return mScale; }
	Float3 GetAngle() { return mAngle; }
	DirectX::XMFLOAT4X4 GetMtx() { return mMtx; }
};