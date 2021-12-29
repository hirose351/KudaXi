#include	"skydome.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"

Skydome::Skydome() :GameObject(("SkyDome"), ObjectType::eDice) {
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/skydome/skydome_pink.fbx",		// ファイル名 
		"shader/vs.hlsl",								// 頂点シェーダー
		"shader/pstexcol.hlsl",							// ピクセルシェーダー
		"assets/model/skydome/");						// テクスチャの格納フォルダ
	if (!sts)
	{
		MessageBox(nullptr, "ColorfulSkydomeモデル 読み込みエラー", "error", MB_OK);
	}
	AddComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/skydome_pink.fbx"));
	ObjectInit();
}

void Skydome::ObjectInit()
{
	// 回転行列作成
	DX11MtxRotationX(0.05f, mRotationMtx);
}

void Skydome::ObjectUpdate()
{
	// 掛け算
	DX11MtxMultiply(mTransform->worldMtx, mRotationMtx, mTransform->worldMtx);
}

void Skydome::ObjectDraw()
{
}

void Skydome::Uninit()
{
}

void Skydome::SetType(Type _type)
{
	switch (_type)
	{
	case Skydome::TITLE:
		GetComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/ColorfulSkydome.fbx"));
		break;
	case Skydome::GAME:
		GetComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/ColorfulSkydome.fbx"));
		break;
	case Skydome::CLEATE:
		GetComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/SelectSkydome.fbx"));
		break;
	}
}