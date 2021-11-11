#pragma once
#include	<directxmath.h>

using namespace DirectX;

class GameObject {
	uint64_t mId = 0;			// ���ʎq
protected:
	XMFLOAT3 mPos;			// �ʒu
	XMFLOAT4X4 mMtx;			// �p��
	XMFLOAT4X4 mLocalMtx;		// ���[�J���̎p��

public:
	GameObject() {}
	virtual ~GameObject() {}
	virtual bool Init() = 0;
	virtual void Finalize() = 0;

	// �p�����擾
	DirectX::XMFLOAT4X4 GetMtx() {
		return mMtx;
	}

	// �C���O�̎p�����擾
	DirectX::XMFLOAT4X4 GetLocalMtx() {
		return mLocalMtx;
	}

	// �ʒu���擾
	DirectX::XMFLOAT3 GetPos() {
		return mPos;
	}

	// �h�c���擾
	uint64_t GetID() {
		return mId;
	}

	void SetMtx(const XMFLOAT4X4 &_mtx) {
		mMtx = _mtx;
	}

	// �����ʒu�Z�b�g
	void SetInitialPos(const XMFLOAT3 &initpos) {
		mPos = initpos;
	}
};