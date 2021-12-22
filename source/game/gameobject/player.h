#pragma once
#include		"gameobject.h"
#include		"dice.h"
#include		"../../system/model/ModelMgr.h"
#include		"../component/allcomponents.h"
#include		"../manager/stagedata_manager.h"
#include		"../component/player_controller.h"

using Microsoft::WRL::ComPtr;

class Player :public GameObject
{
private:
	StageData	stageData;

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
		mTransform->scale = (Float3(4, 6, 4));
		AddComponent<Component::ModelComponent>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/player/player.fbx"));
		AddComponent<Component::CollisionComponent>()->SetLocalScale(mTransform->scale);
		AddComponent<Component::PlayerController>();
		GetComponent<Component::CollisionComponent>()->SetColor(DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 0.01f));
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