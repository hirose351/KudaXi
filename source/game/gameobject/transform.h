#pragma once
#include	"../../system/util/vector.h"

struct Transform
{
	//protected:
	Float3 position;		// �ʒu
	Float3 move;			// �ړ���
	Float3 angle;			// ��]
	Float3 rotation;		// ��]��
	Float3 scale;			// �傫��
	DirectX::XMFLOAT4X4 worldMtx;	// ���[���h�s��
	DirectX::XMFLOAT4X4 localMtx;	// ���[�J���s��
	DirectX::XMFLOAT4X4 scaleMtx;	// �g��s��

	DirectX::XMFLOAT4X4 beforeMtx;	// �g��s��

	Transform();
	virtual ~Transform() {};

	/// Todo:��]�Ƃ��ړ��Ƃ��̊֐����

	// �����l�ɖ߂�
	void ReSetValue();
	// �ړ��ʉ��Z
	void AddPosition();
	// �ړ��ʉ��Z�ړ�
	void MovePosition();
	// �ړ��ʉ��Z
	void AddRotation();
	// �g��s��쐬
	void CreateScaleMtx();
	// �s����쐬
	void CreateMtx();

	void SetPosition(const Float3& _pos) { position = _pos; }
	void SetPositionMove(const Float3& _pos);
	void SetRotation(const Float3& _rot) { rotation = _rot; }
	void SetScale(const Float3& _sc) { scale = _sc; }
	void SetAngle(const Float3& _ang) { angle = _ang; }
	void SetWordMtx(const DirectX::XMFLOAT4X4 _mtx) { worldMtx = _mtx; }
	void PositionCorrectionX(float _pos) {
		worldMtx._41 += _pos;
		position.x += _pos;
	}
	void PositionCorrectionY(float _pos) {
		worldMtx._42 += _pos;
		position.y += _pos;
	}
	void SetPositionXYZ(const Float3& _pos) {
		SetPositionX(_pos.x);
		SetPositionY(_pos.y);
		SetPositionZ(_pos.z);
	}
	void SetPositionX(float _pos) {
		worldMtx._41 = _pos;
		position.x = _pos;
	}
	void SetPositionY(float _pos) {
		worldMtx._42 = _pos;
		position.y = _pos;
	}
	void SetPositionZ(float _pos) {
		worldMtx._43 = _pos;
		position.z = _pos;
	}
	void PositionCorrectionZ(float _pos) {
		worldMtx._43 += _pos;
		position.z += _pos;
	}
	Float3 GetPosition() { return Float3(worldMtx._41, worldMtx._42, worldMtx._43); }
	Float3 GetRotation() { return rotation; }
	Float3 GetScale() { return scale; }
	Float3 GetAngle() { return angle; }
	DirectX::XMFLOAT4X4 GetMtx() { return worldMtx; }
};