#pragma once

#include	<d3d11.h>
#include	<wrl/client.h>
#include	<string>
#include	"../dx11/dx11mathutil.h"
#include	"shaderhashmap.h"

// comptr
using Microsoft::WRL::ComPtr;

// 矩形クラス
class Quad {
private:
	// 頂点データ
	struct Vertex {
		DirectX::XMFLOAT3 pos;		// 座標		
		DirectX::XMFLOAT4 color;	// カラー
		DirectX::XMFLOAT2 tex;		// テクスチャ
	};

	DirectX::XMFLOAT4X4		mWorldmtx;				// ワールド変換行列
	ComPtr<ID3D11Buffer>	mVertexbuffer;			// 頂点バッファ
	ComPtr<ID3D11Buffer>	mIndexbuffer;			// インデックスバッファ
	Quad::Vertex			mVertex[4];				// 矩形４頂点
	DirectX::XMFLOAT4		mColor;					// 頂点カラー
	float					mWidth;					// 幅
	float					mHeight;				// 高さ
	float					mDepth;					// 深度値

	ComPtr<ID3D11ShaderResourceView> mSrv;			// シェーダーリソースビュー
	ComPtr<ID3D11Resource> mResource;				// リソース

	DirectX::XMFLOAT2 mUv[4];						// アニメーション用UVテーブル
	unsigned int mDivu;								// 画像の横分割数
	unsigned int mDivv;								// 画像の縦分割数

public:
	// 矩形の初期化　(width：横描画サイズ　height：縦描画サイズ　tex_name：画像の場所と名前　color：頂点カラー　u：横の分割数　v：縦の分割数　z：奥描画サイズ)
	bool Init(int width, int height, const char *tex_name, const DirectX::XMFLOAT4 &color, int _u = 1, int _v = 1, float z = 0.0f);

	// 描画
	void Draw();

	// テクスチャなしで描画
	void DrawNoTex();

	// 拡大、縮小
	void SetScale(float sx, float sy, float sz);

	// 位置をセット
	void SetPosition(float x, float y, float z = 0.0f);

	// 頂点カラーセット
	void SetColor(const XMFLOAT4 &_color);

	// Z軸回転
	void SetZRotation(float angle);

	// X軸回転
	void SetXRotation(float angle);

	// ワールド変換行列をセットする
	void SetMtx(const DirectX::XMFLOAT4X4 &mtx) {
		mWorldmtx = mtx;
	}

	// 頂点データ更新
	void UpdateVertex(uint32_t width, uint32_t height, const DirectX::XMFLOAT4 &color, float z = 0.0f);

	// 頂点バッファ更新
	void UpdateVbuffer();

	// UV座標設定　(_u：左から何番目か  _v：上から何番目か　direction：描画方向(０→左 １→右)　デフォルトは右)
	void SetTextureUV(int _u, int _v, int direction = 1);
};