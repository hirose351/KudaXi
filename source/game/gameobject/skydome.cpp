#include	"skydome.h"
#include	"../../system/model/model_manager.h"
#include	"../component/allcomponents.h"

Skydome::Skydome() :GameObject(("SkyDome"), ObjectType::eDice, false) {
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/skydome/skydome_pink.fbx",		// �t�@�C���� 
		"shader/vs.hlsl",								// ���_�V�F�[�_�[
		"shader/pstexcol.hlsl",							// �s�N�Z���V�F�[�_�[
		"assets/model/skydome/");						// �e�N�X�`���̊i�[�t�H���_
	if (!sts)
	{
		MessageBox(nullptr, "skydome_pink���f�� �ǂݍ��݃G���[", "error", MB_OK);
	}
	sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/skydome/skydome_select.fbx",		// �t�@�C���� 
		"shader/vs.hlsl",								// ���_�V�F�[�_�[
		"shader/pstexcol.hlsl",							// �s�N�Z���V�F�[�_�[
		"assets/model/skydome/");						// �e�N�X�`���̊i�[�t�H���_
	if (!sts)
	{
		MessageBox(nullptr, "skydome_select���f�� �ǂݍ��݃G���[", "error", MB_OK);
	}
	AddComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/skydome_pink.fbx"));
	GetComponent<Component::Model>()->SetOrderInLayer(-1000);
	ObjectInit();
}

void Skydome::ObjectInit()
{
	// ��]�s��쐬
	DX11MtxRotationX(0.05f, mRotationMtx);
}

void Skydome::ObjectUpdate()
{
	// �|���Z
	DX11MtxMultiply(mTransform->worldMtx, mRotationMtx, mTransform->worldMtx);
}

void Skydome::Uninit()
{
}

void Skydome::SetType(Type _type)
{
	switch (_type)
	{
	case Skydome::TITLE:
		GetComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/skydome_pink.fbx"));
		break;
	case Skydome::GAME:
		GetComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/skydome_pink.fbx"));
		break;
	case Skydome::CLEATE:
		GetComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/skydome_select.fbx"));
		break;
	}
}