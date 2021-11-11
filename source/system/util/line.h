#pragma once
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<d3d11.h>
#include	<vector>
#include	"../dx11/util/DX11util.h"
#include	"../dx11/Shader.h"

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

	bool Init(const std::vector<MyVertex>& v) {
		mVsize = v.size();

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		unsigned int numElements = ARRAYSIZE(layout);

		// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
		bool sts = CreateVertexShader(
			GetDX11Device(),
			"shader/vsline.hlsl",
			"main",
			"vs_5_0",
			layout,
			numElements,
			&mVs,
			&mLayout);
		if (!sts)
		{
			MessageBox(nullptr, "CreateVertexShader(Line) error", "error", MB_OK);
			return false;
		}

		// ピクセルシェーダーを生成
		sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
								GetDX11Device(),				// デバイスオブジェクト
								"shader/psline.hlsl",
								"main",
								"ps_5_0",
								&mPs);
		if (!sts)
		{
			MessageBox(nullptr, "CreatePixelShader(Line) error", "error", MB_OK);
			return false;
		}

		// 頂点バッファ生成
		sts = CreateVertexBufferWrite(GetDX11Device(), static_cast<unsigned int>(sizeof(MyVertex)), static_cast<unsigned int>(v.size()), (void*)v.data(), &mVbuffer);
		if (!sts)
		{
			MessageBox(nullptr, "CreateVertexBufferWrite(Line) error", "error", MB_OK);
			return false;
		}

		return true;
	}

	void Dispose() {
	}

	void SetVertex(std::vector<MyVertex>& v) {

		D3D11_MAPPED_SUBRESOURCE pData;

		HRESULT hr = GetDX11DeviceContext()->Map(mVbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr))
		{
			memcpy_s(pData.pData, pData.RowPitch, (void*)(v.data()), sizeof(MyVertex) * v.size());
			GetDX11DeviceContext()->Unmap(mVbuffer.Get(), 0);
		}

		mVsize = v.size();
	}

	void Draw() {
		ID3D11DeviceContext* devcontext = GetDX11DeviceContext();

		// 頂点バッファをセットする
		unsigned int stride = sizeof(MyVertex);
		unsigned  offset = 0;
		devcontext->IASetVertexBuffers(0, 1, mVbuffer.GetAddressOf(), &stride, &offset);

		devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);		// トポロジーをセット（旧プリミティブタイプ）
		devcontext->IASetInputLayout(mLayout.Get());			// 頂点レイアウトセット

		devcontext->VSSetShader(mVs.Get(), nullptr, 0);		// 頂点シェーダーをセット
		devcontext->PSSetShader(mPs.Get(), nullptr, 0);		// ピクセルシェーダーをセット

		devcontext->Draw(
			static_cast<UINT>(mVsize),							// 頂点数
			0);									// 頂点バッファの最初から使う
	}
};