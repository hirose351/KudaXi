#include "transform.h"

Transform::Transform()
{
	DX11MtxIdentity(mtx);	// íPà çsóÒâª
}

void Transform::AddPosition()
{
	position += move;
}

void Transform::CreateScaleMtx()
{
	DirectX::XMFLOAT4X4 mScaleMtx;
	DX11MtxScale(7.0f, 7.0f, 7.0f, mScaleMtx);
	DX11MtxMultiply(mtx, mScaleMtx, mtx);
}