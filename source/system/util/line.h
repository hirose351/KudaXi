#pragma once
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<d3d11.h>
#include	<vector>
#include	"../dx11/DX11util.h"

using Microsoft::WRL::ComPtr;

class Line {
private:
	ComPtr<ID3D11Buffer>		mVbuffer;			// 頂点バッファ
	ComPtr<ID3D11PixelShader>	mPs;				// ピクセルシェーダー
	ComPtr<ID3D11VertexShader>	mVs;				// 頂点シェーダー
	ComPtr<ID3D11InputLayout>   mLayout;			// 頂点フォーマット定義
	size_t						mVsize;			// 頂点数
public:
	struct MyVertex {
		DirectX::XMFLOAT3		vertex;
		DirectX::XMFLOAT4		color;
	};

	Line() {

	}

	~Line() {

	}

	bool Init(const std::vector<MyVertex>& v);

	void Dispose() {}

	void SetVertex(std::vector<MyVertex>& v);

	void Draw();
};