#include	"skydome.h"
#include	"../../system/dx11/util/dx11mathutil.h"
#include	"../../system/model/ModelMgr.h"

Skydome::Skydome()
{
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/skydome/ColorfulSkydome.fbx",		// �t�@�C���� 
		"shader/vs.hlsl",								// ���_�V�F�[�_�[
		"shader/pstexcol.hlsl",							// �s�N�Z���V�F�[�_�[
		"assets/model/skydome/");						// �e�N�X�`���̊i�[�t�H���_
	if (!sts)
	{
		MessageBox(nullptr, "ColorfulSkydome���f�� �ǂݍ��݃G���[", "error", MB_OK);
	}

	sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/skydome/SelectSkydome.fbx",		// �t�@�C���� 
		"shader/vs.hlsl",								// ���_�V�F�[�_�[
		"shader/pstexcol.hlsl",							// �s�N�Z���V�F�[�_�[
		"assets/model/skydome/");						// �e�N�X�`���̊i�[�t�H���_
	if (!sts)
	{
		MessageBox(nullptr, "SelectSkydome���f�� �ǂݍ��݃G���[", "error", MB_OK);
	}

	XMFLOAT4X4 scaleMtx;	// �g��p�s��
	// �s�񏉊���
	DX11MtxIdentity(mMtx);
	// �g��s��쐬
	DX11MtxScale(15, 15, 15, scaleMtx);
	// �|���Z
	DX11MtxMultiply(mMtx, scaleMtx, mMtx);
}

void Skydome::SetType(Type _type)
{
	switch (_type)
	{
	case Skydome::TITLE:
		SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/ColorfulSkydome.fbx"));
		break;
	case Skydome::GAME:
		SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/ColorfulSkydome.fbx"));
		break;
	case Skydome::CLEATE:
		SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/SelectSkydome.fbx"));
		break;
	}
}

bool Skydome::Init() {

	//SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/ColorfulSkydome.fbx"));
	return true;
}

void Skydome::Draw() {

	// ���f���`��
	mPmodel->Draw(mMtx);
}

void Skydome::Update() {

	XMFLOAT4X4 rotationMtx;	//��]�p�s��

	// ��]�s��쐬
	DX11MtxRotationX(0.07f, rotationMtx);
	// �|���Z
	DX11MtxMultiply(mMtx, rotationMtx, mMtx);

	mMtx._41 = 0;
	mMtx._42 = 0;
	mMtx._43 = 0;
}

void Skydome::Finalize() {
	mPmodel->Uninit();
}
