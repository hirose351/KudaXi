#pragma once
#include	"gameobject.h"
#include	"transform.h"
#include	"gameobject_utility.h"
#include	"../../system/model/CModel.h"

class Player :public GameObject, Transform
{
private:
	CModel*	mpModel;							// ３Ｄモデル
	Transform mTramsform;						// 位置、姿勢、大きさ
	bool mIsDiceMove = false;					// サイコロが回転しているか
	DIRECTION mDiceMoveDirection;				// サイコロを回転させる方向
	DIRECTION mDirection;						// プレイヤーの方向(キー参照)

	///////////////////////////////////////////////////////////////

	Vector3					mMove;					// 移動量
	Vector3					mDestrot;				// 目標姿勢
	DIRECTION				mMoveKeySts;			// 押されている移動キー


	// モデルポインタ取得
	void SetModel(CModel* p) {
		mpModel = p;
	}
public:
	Player();
	~Player();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize()override;
};