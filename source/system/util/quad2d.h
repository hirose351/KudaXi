#pragma once
#include	<d3d11.h>
#include	<wrl/client.h>
#include	<string>
#include	"../dx11/dx11_mathutil.h"
#include	"shaderhashmap.h"
#include	"texture_manager.h"
#include	"vector.h"

// comptr
using Microsoft::WRL::ComPtr;

// 描画するタイプ
enum class DrawType {
	eTex,
	eNoTex,
	eMokomokoShaderTex
};

// 矩形クラス
class CQuad2D {
private:
	// 頂点データ
	struct Vertex {
		DirectX::XMFLOAT3 pos;		// 座標		
		DirectX::XMFLOAT4 color;	// カラー
		DirectX::XMFLOAT2 tex;		// テクスチャ
	};

	ComPtr<ID3D11Buffer>	mVertexbuffer;			// 頂点バッファ
	ComPtr<ID3D11Buffer>	mIndexbuffer;			// インデックスバッファ
	CQuad2D::Vertex			mVertex[4];				// 矩形４頂点
	DirectX::XMFLOAT4		mColor;					// 頂点カラー
	DirectX::XMFLOAT2		mScale;					// 大きさ
	float					mDepth;					// 深度値

	TextureInfo mTexInfo;

	DirectX::XMFLOAT2 mUv[4];						// アニメーション用UVテーブル
	unsigned int mDivu;								// 画像の横分割数
	unsigned int mDivv;								// 画像の縦分割数

	ALIGN16 struct ConstantBufferTime {
		float time;
	};

	ID3D11Buffer*       mpConstantBufferTime = nullptr;

public:
	// 矩形の初期化　(_scale：大きさ　_texName：画像の場所と名前　_color：頂点カラー　_u：横の分割数　_v：縦の分割数　_z：奥描画サイズ)
	bool Init(DirectX::XMFLOAT2 _scale, std::string _texName, const DirectX::XMFLOAT4 &_color, int _u = 1, int _v = 1, float _z = 0.0f);

	// 描画
	void Draw(DirectX::XMFLOAT4X4 _mtx);

	// テクスチャなしで描画
	void DrawNoTex(DirectX::XMFLOAT4X4 _mtx);

	// 特殊シェーダーで描画
	void DrawMokoMokoShaderTex(DirectX::XMFLOAT4X4 _mtx);

	// 頂点カラーセット
	void SetColor(const XMFLOAT4 &_color);

	// 頂点データ更新
	void UpdateVertex(DirectX::XMFLOAT2 _scale, const DirectX::XMFLOAT4 &color, float z = 0.0f);

	// 頂点バッファ更新
	void UpdateVbuffer();

	// UV座標設定　(_u：左から何番目か  _v：上から何番目か　direction：描画方向(０→左 １→右)　デフォルトは右)
	void SetTextureUV(int _u, int _v, int direction = 1);
};