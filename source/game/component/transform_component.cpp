#include "transform_component.h"

TransformComponent::TransformComponent()
{
	ReSetValue();	// íPà çsóÒâª
}

void TransformComponent::ReSetValue()
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

void TransformComponent::AddPosition()
{
	position += move;
}

void TransformComponent::CreateScaleMtx()
{
	DirectX::XMFLOAT4X4 mScaleMtx;
	DX11MtxScale(7.0f, 7.0f, 7.0f, mScaleMtx);
	DX11MtxMultiply(mtx, mScaleMtx, mtx);
}

void TransformComponent::Update()
{

}

void TransformComponent::Draw()
{

}
