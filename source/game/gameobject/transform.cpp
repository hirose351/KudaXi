#include "transform.h"

Transform::Transform()
{
	ReSetValue();	// íPà çsóÒâª
}

void Transform::ReSetValue()
{
	position = move = rotation = angle = 0;
	scale = 1;

	DX11MtxIdentity(worldMtx);
	DX11MtxIdentity(localMtx);
	DX11MtxIdentity(scaleMtx);
}

void Transform::AddPosition()
{
	position += move;
}

void Transform::MovePosition()
{
	position += move;
	worldMtx._41 = position.x;
	worldMtx._42 = position.y;
	worldMtx._43 = position.z;
}

void Transform::AddRotation()
{
}

void Transform::CreateScaleMtx()
{
	DirectX::XMFLOAT4X4 mScaleMtx;
	DX11MtxScale(scale.x, scale.y, scale.z, mScaleMtx);
	DX11MtxMultiply(worldMtx, mScaleMtx, worldMtx);
}

void Transform::CreateWordMtx()
{
	DX11MakeWorldMatrix(worldMtx, angle, position);
}

void Transform::CreateLocalMtx()
{
	DX11MakeWorldMatrix(localMtx, angle, position);
}

void Transform::SetPositionMove(const Float3 & _pos)
{
	position = _pos;
	worldMtx._41 = _pos.x;
	worldMtx._42 = _pos.y;
	worldMtx._43 = _pos.z;
}
