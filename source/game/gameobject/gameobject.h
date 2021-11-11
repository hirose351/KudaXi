#pragma once
#include	<directxmath.h>

using namespace DirectX;

class GameObject {
	uint64_t mId = 0;			// 識別子
protected:
	XMFLOAT3 mPos;			// 位置
	XMFLOAT4X4 mMtx;			// 姿勢
	XMFLOAT4X4 mLocalMtx;		// ローカルの姿勢

public:
	GameObject() {}
	virtual ~GameObject() {}
	virtual bool Init() = 0;
	virtual void Finalize() = 0;

	// 姿勢を取得
	DirectX::XMFLOAT4X4 GetMtx() {
		return mMtx;
	}

	// 修正前の姿勢を取得
	DirectX::XMFLOAT4X4 GetLocalMtx() {
		return mLocalMtx;
	}

	// 位置を取得
	DirectX::XMFLOAT3 GetPos() {
		return mPos;
	}

	// ＩＤを取得
	uint64_t GetID() {
		return mId;
	}

	void SetMtx(const XMFLOAT4X4 &_mtx) {
		mMtx = _mtx;
	}

	// 初期位置セット
	void SetInitialPos(const XMFLOAT3 &initpos) {
		mPos = initpos;
	}
};