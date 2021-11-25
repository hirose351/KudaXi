#pragma once
#include	"gameobject.h"
#include	"collision_base.h"
#include	"../../system/model/ModelMgr.h"

using Microsoft::WRL::ComPtr;

class Player :public GameObject, CollisionInterface
{
private:
	CModel*	mpModel;							// �R�c���f��
	DIRECTION mDiceMoveDirection;				// �T�C�R������]���������
	bool mIsDiceMove = false;					// �T�C�R������]���Ă��邩
	DIRECTION mDirection;						// �v���C���[�̕���(�L�[�Q��)

	Float3					mDestrot;			// �ڕW�p��
	DIRECTION				mMoveKeySts;		// ������Ă���ړ��L�[

	// ���f���|�C���^�擾
	void SetModel(CModel* p) {
		mpModel = p;
	}
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
		SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/player/player.fbx"));
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