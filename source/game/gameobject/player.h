#pragma once
#include	"gameobject.h"
#include	"dice.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"
#include	"../state/state.h"
#include	"../manager/stagedata_manager.h"

using Microsoft::WRL::ComPtr;

class Player :public GameObject
{
private:
	Direction mDiceMoveDirection;				// サイコロを回転させる方向
	bool mIsDiceMove = false;					// サイコロが回転しているか
	Direction mDirection;						// プレイヤーの方向(キー参照)

	Float3					mDestrot;			// 目標姿勢
	Direction				mMoveKeySts;		// 押されている移動キー

	Dice* mpOperationDice;						// 操作中のサイコロ

	Pstate mPstate = eStop;

	Float3 mInitMapPos = (2.0f, 0, 1.0f);		// 初期マップ位置
	int mStartCount = 150;						// 開始時の停止時間(実際は上がってるサイコロの状態で移動制限されるので必要ない)
	StageData stageData;
	void Move();
	void Roll();
	void Push();
	void CheckRoll();
	void StageHitCorrection();

	// 最も近いサイコロを登録
	bool SetNearestDice();
public:
	Player() : GameObject(("Player"), ObjectType::ePlayer) {
		bool sts = ModelMgr::GetInstance().LoadModel(
			"assets/model/player/player.fbx",
			"shader/vs.hlsl", "shader/ps.hlsl",
			"assets/model/player/");
		if (!sts)
		{
			MessageBox(nullptr, "Playerモデル 読み込みエラー", "error", MB_OK);
		}
		AddComponent<Component::ModelComponent>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/player/player.fbx"));
		AddComponent<Component::CollisionComponent>()->SetLocalScale(Float3(4, 6, 4));
		GetComponent<Component::CollisionComponent>()->SetColor(DirectX::XMFLOAT4(1, 0, 0, 0.3f));
		ObjectInit();
	};
	~Player();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectDraw()override {};
	void ObjectImguiDraw()override;
	void Uninit() override;

	void OnCollisionEnter(ComponentBase* _oher) override;
	void OnCollisionStay(ComponentBase* _oher) override;
	void OnCollisionExit(ComponentBase* _oher) override;

	/// Todo:ダブルディスパッチのやり方間違えてるので直す
	void OnColEnterObj(Dice* _other);
	void OnColStayObj(Dice* _other);
	void OnColExitObj(Dice* _other);
};