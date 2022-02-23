#pragma once
#include	"draw_component_base.h"
#include	"../../system/model/vertexproto.h"
#include	"../../system/util/texture_manager.h"

using Microsoft::WRL::ComPtr;

enum class PlaneTextueName {
	eBlack,
	eKago,
};

namespace Component {
	class Plane : public DrawComponentBase
	{
	private:
		// todo:ステージによってテクスチャ分ける
		const char*	mTexFileName[4] =
		{
			"assets/image/stage/BLACK.png",
			"assets/image/stage/STAGE.png",
			"assets/image/stage/STAGE.png",
			"assets/image/stage/STAGE.png",
		};

		Transform mTransform;
		TextureInfo mTexInfo[4];
		int mTexInfoNum = 1;

		ComPtr<ID3D11Buffer> mpVertexBuffer;		// 頂点バッファ
		ComPtr<ID3D11VertexShader> mpVertexShader;	// 頂点シェーダー
		ComPtr<ID3D11PixelShader>  mpPixelShader;	// ピクセルシェーダー
		ComPtr<ID3D11InputLayout>  mpVertexLayout;	// 頂点レイアウト

		Float3 mScale = (DICE_SCALE - 0.8f, 0, DICE_SCALE - 0.8f);

		// 平面チップ
		Vertex mVartex[4] = {
			// 頂点座標															法線ベクトル								テクスチャ座標
			{ DirectX::XMFLOAT3(-mScale.x / 2.0f, 0,  mScale.z / 2.0f), DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT2(1.0f, 0.0f) },
			{ DirectX::XMFLOAT3(mScale.x / 2.0f, 0,  mScale.z / 2.0f), DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT2(1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-mScale.x / 2.0f, 0, -mScale.z / 2.0f), DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT2(0.0f, 0.0f) },
			{ DirectX::XMFLOAT3(mScale.x / 2.0f, 0, -mScale.z / 2.0f), DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT2(0.0f, 1.0f) }
		};

	public:
		Plane();

		void Init() override;
		void Update()override;
		void Draw()override;
		void ImguiDrawComponent()override {}
		void Uninit()override;

		void SetScale(Float3 _scale) { mScale = _scale; }
		void SetTextureName(PlaneTextueName _pName) { mTexInfoNum = static_cast<int>(_pName); }
	};
}