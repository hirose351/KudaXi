#pragma once
#include	"gameobject.h"
#include	"transform.h"
#include	"../../system/model/vertexproto.h"
#include	"../../system/util/texture_manager.h"

class Plane : public GameObject, Transform
{
private:
	TextureInfo* mTexInfo[4];

	ComPtr<ID3D11Buffer> mpVertexBuffer;		// 頂点バッファ
	ComPtr<ID3D11Buffer> mpIndexBuffer;			// インデックスバッファ
	ComPtr<ID3D11VertexShader> mpVertexShader;	// 頂点シェーダー
	ComPtr<ID3D11PixelShader>  mpPixelShader;	// ピクセルシェーダー
	ComPtr<ID3D11InputLayout>  mpVertexLayout;	// 頂点レイアウト

	int mMapChipSize = static_cast<int>(DICESCALE);

	// 平面チップ
	Vertex mVartex[4] = {
		// 頂点座標															法線ベクトル								テクスチャ座標
		{ DirectX::XMFLOAT3(-mMapChipSize / 2.0f, 0,  mMapChipSize / 2.0f), DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(mMapChipSize / 2.0f, 0,  mMapChipSize / 2.0f), DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-mMapChipSize / 2.0f, 0, -mMapChipSize / 2.0f), DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(mMapChipSize / 2.0f, 0, -mMapChipSize / 2.0f), DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT2(0.0f, 1.0f) }
	};

	// todo:ステージによってテクスチャ分ける
	const char*	mTexFileName[4] =
	{
		"assets/image/stage/BLACK.png",
		"assets/image/stage/STAGE.png",
		"assets/image/stage/STAGE.png",
		"assets/image/stage/STAGE.png",
	};

public:
	Plane() :GameObject(("Plane"), ObjectType::Obstracle) {};
	~Plane()override {};

	void Init() override;
	void Update()override;
	void Draw()override;
	void Finalize()override;
};

