#pragma once
#include	"gameobject.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"
#include	"../../system/util/INT3.h"

class Skydome : public GameObject {
private:
	const float mRotationNum = 0.07f;
	DirectX::XMFLOAT4X4 mRotationMtx;	//��]�p�s��

public:
	enum Type {
		TITLE,
		GAME,
		CLEATE,
	};

	Skydome() :GameObject(("SkyDome"), ObjectType::eDice) {
		bool sts = ModelMgr::GetInstance().LoadModel(
			"assets/model/skydome/skydome_pink.fbx",		// �t�@�C���� 
			"shader/vs.hlsl",								// ���_�V�F�[�_�[
			"shader/pstexcol.hlsl",							// �s�N�Z���V�F�[�_�[
			"assets/model/skydome/");						// �e�N�X�`���̊i�[�t�H���_
		if (!sts)
		{
			MessageBox(nullptr, "ColorfulSkydome���f�� �ǂݍ��݃G���[", "error", MB_OK);
		}
		AddComponent<Component::ModelComponent>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/skydome_pink.fbx"));
		ObjectInit();
	}

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectDraw()override;
	void Uninit()override;

	void SetType(Type _type);
};