#include	"skydome.h"
#include	"../../system/dx11/util/dx11mathutil.h"
#include	"../../system/model/ModelMgr.h"

Skydome::Skydome()
{
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/skydome/ColorfulSkydome.fbx",		// ファイル名 
		"shader/vs.hlsl",								// 頂点シェーダー
		"shader/pstexcol.hlsl",							// ピクセルシェーダー
		"assets/model/skydome/");						// テクスチャの格納フォルダ
	if (!sts)
	{
		MessageBox(nullptr, "ColorfulSkydomeモデル 読み込みエラー", "error", MB_OK);
	}

	sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/skydome/SelectSkydome.fbx",		// ファイル名 
		"shader/vs.hlsl",								// 頂点シェーダー
		"shader/pstexcol.hlsl",							// ピクセルシェーダー
		"assets/model/skydome/");						// テクスチャの格納フォルダ
	if (!sts)
	{
		MessageBox(nullptr, "SelectSkydomeモデル 読み込みエラー", "error", MB_OK);
	}

	XMFLOAT4X4 scaleMtx;	// 拡大用行列
	// 行列初期化
	DX11MtxIdentity(mMtx);
	// 拡大行列作成
	DX11MtxScale(15, 15, 15, scaleMtx);
	// 掛け算
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

	// モデル描画
	mPmodel->Draw(mMtx);
}

void Skydome::Update() {

	XMFLOAT4X4 rotationMtx;	//回転用行列

	// 回転行列作成
	DX11MtxRotationX(0.07f, rotationMtx);
	// 掛け算
	DX11MtxMultiply(mMtx, rotationMtx, mMtx);

	mMtx._41 = 0;
	mMtx._42 = 0;
	mMtx._43 = 0;
}

void Skydome::Finalize() {
	mPmodel->Uninit();
}
