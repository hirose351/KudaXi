#pragma once
#include	"gameobject.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"
#include	"../../system/util/INT3.h"

class Skydome : public GameObject {
private:
	const float mRotationNum = 0.07f;
	DirectX::XMFLOAT4X4 mRotationMtx;	//回転用行列

public:
	enum Type {
		TITLE,
		GAME,
		CLEATE,
	};

	Skydome() :GameObject(("SkyDome"), ObjectType::eDice) {
		bool sts = ModelMgr::GetInstance().LoadModel(
			"assets/model/skydome/skydome_pink.fbx",		// ファイル名 
			"shader/vs.hlsl",								// 頂点シェーダー
			"shader/pstexcol.hlsl",							// ピクセルシェーダー
			"assets/model/skydome/");						// テクスチャの格納フォルダ
		if (!sts)
		{
			MessageBox(nullptr, "ColorfulSkydomeモデル 読み込みエラー", "error", MB_OK);
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