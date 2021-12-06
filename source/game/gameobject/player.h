#pragma once
#include	"gameobject.h"
#include	"dice.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"

using Microsoft::WRL::ComPtr;

class Player :public GameObject/*, CollisionInterface*/
{
private:
	DIRECTION mDiceMoveDirection;				// �T�C�R������]���������
	bool mIsDiceMove = false;					// �T�C�R������]���Ă��邩
	DIRECTION mDirection;						// �v���C���[�̕���(�L�[�Q��)

	Float3					mDestrot;			// �ڕW�p��
	DIRECTION				mMoveKeySts;		// ������Ă���ړ��L�[

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
			MessageBox(nullptr, "Player���f�� �ǂݍ��݃G���[", "error", MB_OK);
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