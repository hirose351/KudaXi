#pragma once
#include	"gameobject.h"
#include	"transform.h"
#include	"collision_base.h"
#include	"../../system/model/ModelMgr.h"

using Microsoft::WRL::ComPtr;

class Player :public GameObject, Transform, CollisionBase
{
private:
	CModel*	mpModel;							// ３Ｄモデル
	//Transform mTramsform;						// 位置、姿勢、大きさ
	bool mIsDiceMove = false;					// サイコロが回転しているか
	DIRECTION mDiceMoveDirection;				// サイコロを回転させる方向
	DIRECTION mDirection;						// プレイヤーの方向(キー参照)

	Float3					mDestrot;			// 目標姿勢
	DIRECTION				mMoveKeySts;		// 押されている移動キー

	// モデルポインタ取得
	void SetModel(CModel* p) {
		mpModel = p;
	}
public:
	Player() :CollisionBase(new Primitive::Capsule, this), GameObject(("Player"), ObjectType::Player) {
		bool sts = ModelMgr::GetInstance().LoadModel(
			"assets/model/player/player.fbx",
			"shader/vs.hlsl", "shader/ps.hlsl",
			"assets/model/player/");
		if (!sts)
		{
			MessageBox(nullptr, "Playerモデル 読み込みエラー", "error", MB_OK);
		}
		SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/player/player.fbx"));
		DX11MtxIdentity(mMtx);	// 単位行列化
		Init();
	};
	~Player();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

	void OnCollisionEnter() override;
	void OnCollisionStay() override;
	void OnCollisionExit() override;
};