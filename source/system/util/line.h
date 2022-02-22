#pragma once
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<d3d11.h>
#include	<vector>
#include	"../dx11/dx11_util.h"

using Microsoft::WRL::ComPtr;

class Line {
private:
	ComPtr<ID3D11Buffer>		mpVbuffer;			// 頂点バッファ
	ComPtr<ID3D11PixelShader>	mpPs;				// ピクセルシェーダー
	ComPtr<ID3D11VertexShader>	mpVs;				// 頂点シェーダー
	ComPtr<ID3D11InputLayout>   mpLayout;			// 頂点フォーマット定義
	size_t						mVsize;				// 頂点数
public:
	struct MyVertex
	{
		DirectX::XMFLOAT3		vertex;
		DirectX::XMFLOAT4		color;
	};

	Line() {}
	~Line() {}

	bool Init(const std::vector<MyVertex>& v);
	void SetVertex(std::vector<MyVertex>& v);
	void Draw();
	void Dispose() {}
};