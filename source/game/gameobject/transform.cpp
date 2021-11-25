#include "transform.h"

Transform::Transform()
{
	DX11MtxIdentity(mMtx);	// íPà çsóÒâª
}

void Transform::AddPosition()
{
	mPosition += mMove;
}

void Transform::CreateScaleMtx()
{
	DirectX::XMFLOAT4X4 mScaleMtx;
	DX11MtxScale(7.0f, 7.0f, 7.0f, mScaleMtx);
	DX11MtxMultiply(mMtx, mScaleMtx, mMtx);
}