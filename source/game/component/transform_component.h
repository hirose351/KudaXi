#pragma once
#include "component_base.h"

class TransformComponent : public ComponentBase
{
	Float3 position;
	Float3 move;
	Float3 rotation;
	Float3 scale;
	Float3 angle;
	DirectX::XMFLOAT4X4 mtx;
	DirectX::XMFLOAT4X4 localMtx;
	DirectX::XMFLOAT4X4 scaleMtx;

	TransformComponent();

	// Todo:âÒì]Ç∆Ç©à⁄ìÆÇ∆Ç©ÇÃä÷êîçÏÇÈ

	void ReSetValue();
	void AddPosition();
	void CreateScaleMtx();

	void SetPosition(const Float3& _pos) { position = _pos; }
	void SetRotation(const Float3& _rot) { rotation = _rot; }
	void SetScale(const Float3& _sc) { scale = _sc; }
	void SetAngle(const Float3& _ang) { angle = _ang; }
	void SetMtx(const DirectX::XMFLOAT4X4 _mtx) { mtx = _mtx; }
	Float3 GetPosition() { return position; }
	Float3 GetRotation() { return rotation; }
	Float3 GetScale() { return scale; }
	Float3 GetAngle() { return angle; }
	DirectX::XMFLOAT4X4 GetMtx() { return mtx; }

	void Update()override;
	void Draw()override;
};