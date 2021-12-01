#pragma once
#include	<wrl/client.h>
#include	"../../system/dx11/Shader.h"
#include	"../gameobject/primitive.h"
#include	"../gameobject/transform.h"

constexpr float PI = DirectX::XM_PI;

using Microsoft::WRL::ComPtr;

class Qube {
private:
	ComPtr<ID3D11Buffer> mpVertexBuffer;		// 頂点バッファ
	ComPtr<ID3D11Buffer> mpIndexBuffer;		// インデックスバッファ
	ComPtr<ID3D11VertexShader> mpVertexShader;	// 頂点シェーダー
	ComPtr<ID3D11PixelShader> mpPixelShader;	// ピクセルシェーダー
	ComPtr<ID3D11InputLayout>  mpVertexLayout;	// 頂点レイアウト
	Primitive::AABB mAabb;
	XMFLOAT4			mColor;				// 頂点カラー
	// 頂点データ
	struct Vertex {
		DirectX::XMFLOAT3	Pos;
		DirectX::XMFLOAT3	Normal;
		DirectX::XMFLOAT4	Color;
	};

	// インデックスデータ
	struct Face {
		unsigned int idx[3];
	};

	std::vector<Vertex>		mVertex;			//　頂点データ
	std::vector<Face>		mFace;				//	インデックスデータ

	// インデックスデータを作成する
	void CreateIndex();
	// 頂点データを作成する
	void CreateVertex();
	// 正規化
	void Normalize(XMFLOAT3 _vector, XMFLOAT3& _Normal);
public:
	// 初期化
	bool Init(const Primitive::AABB& _aabb, const XMFLOAT4& color);
	// 更新
	void Update(const Primitive::AABB& _aabb);
	// 描画
	void Draw(DirectX::XMFLOAT4X4 _mtx);
};

