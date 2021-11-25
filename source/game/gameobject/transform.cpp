#include "transform.h"

Transform::Transform()
{
	ReSetValue();	// íPà çsóÒâª
}

void Transform::ReSetValue()
{
	position = { 0, 0, 0 };
	move = { 0, 0, 0 };
	rotation = { 0, 0, 0 };
	scale = { 0, 0, 0 };
	angle = { 0, 0, 0 };
	DX11MtxIdentity(mtx);
	DX11MtxIdentity(localMtx);
	DX11MtxIdentity(scaleMtx);
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