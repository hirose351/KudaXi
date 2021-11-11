#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<math.h>
#include	<vector>
#include	<wrl/client.h>
#include	"../dx11/Shader.h"

constexpr float PI = DirectX::XM_PI;

using Microsoft::WRL::ComPtr;

class Sphere {
private:
	ComPtr<ID3D11Buffer> mpVertexBuffer;		// 頂点バッファ
	ComPtr<ID3D11Buffer> mpIndexBuffer;		// インデックスバッファ
	ComPtr<ID3D11VertexShader> mpVertexShader;	// 頂点シェーダー
	ComPtr<ID3D11PixelShader> mpPixelShader;	// ピクセルシェーダー
	ComPtr<ID3D11InputLayout>  mpVertexLayout;	// 頂点レイアウト
	unsigned int		mDivX;					// 水平方向の分割数
	unsigned int		mDivY;					// 垂直方向の分割数
	float				mRadius;				// 半径
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
	// 描画
	void Draw(ID3D11DeviceContext* _device);

	// 初期化
	bool Init(float r,						// 半径
			  int _divisionHorizontal,			// 水平方向の分割数
			  int _divisionVertical,				// 垂直方向の分割数
			  XMFLOAT4	_color,					// 頂点カラー
			  ID3D11Device* _device);
};