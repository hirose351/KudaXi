#pragma once
#include	"gameobject.h"
#include	"dice.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"

using Microsoft::WRL::ComPtr;

class Player :public GameObject/*, CollisionInterface*/
{
private:
	DIRECTION mDiceMoveDirection;				// サイコロを回転させる方向
	bool mIsDiceMove = false;					// サイコロが回転しているか
	DIRECTION mDirection;						// プレイヤーの方向(キー参照)

	Float3					mDestrot;			// 目標姿勢
	DIRECTION				mMoveKeySts;		// 押されている移動キー

	bool isDiceOperation = false;
	Dice* mdice;
public:
	Player() : GameObject(("Player"), ObjectType::Player) {
		bool sts = ModelMgr::GetInstance().LoadModel(
			"assets/model/player/player.fbx",
			"shader/vs.hlsl", "shader/ps.hlsl",
			"assets/model/player/");
		if (!sts)
		{
			MessageBox(nullptr, "Playerモデル 読み込みエラー", "error", MB_OK);
		}
		mTransform.scale = 2;
		AddComponent<Component::ModelComponent>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/player/player.fbx"));
		//AddComponent<Component::CollisionComponent>()->SetLocalPos(20);
		AddComponent<Component::CollisionComponent>()->SetLocalScale(Float3(4, 7, 4));
		GetComponent<Component::CollisionComponent>()->SetColor(DirectX::XMFLOAT4(1, 0, 0, 0.3f));
		ObjectInit();
	};
	~Player();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectDraw()override {};
	void Uninit() override;

	void OnCollisionEnter(ComponentBase* _oher) override;
	void OnCollisionStay(ComponentBase* _oher) override;
	void OnCollisionExit(ComponentBase* _oher) override;

	void OnColEnterObj(Dice* _other);
	void OnColStayObj(Dice* _other);
	void OnColExitObj(Dice* _other);
};