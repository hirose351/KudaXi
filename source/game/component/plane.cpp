#include	"plane.h"
#include	"../../system/dx11/dx11_util.h"
#include	"../../system/dx11/dx11_settransform.h"
#include	"../../system/dx11/shader.h"

using namespace Component;

Plane::Plane()
{
	ID3D11Device* dev;
	ID3D11DeviceContext* devcontext;

	dev = GetDX11Device();
	devcontext = GetDX11DeviceContext();

	// テクスチャ読み込み
	for (int i = 0; i < 4; i++)
	{
		mTexInfo[i] = *TextureManager::GetInstance().GetTexturePtr(mTexFileName[i]);
	}

	bool sts = CreateVertexBuffer(dev, sizeof(Vertex), 4, mVartex, &mpVertexBuffer);
	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexBuffer error(stage)", "error", MB_OK);
	}

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	sts = CreateVertexShader(dev, "shader/vs.hlsl", "main", "vs_5_0", layout, numElements, &mpVertexShader, &mpVertexLayout);
	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexShader error(stage)", "error", MB_OK);
	}

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(dev, "shader/pstexcol.hlsl", "main", "ps_5_0", &mpPixelShader);
	if (!sts)
	{
		MessageBox(nullptr, "CreatePixelShader error(stage)", "error", MB_OK);
	}
}

void Plane::Init()
{
}

void Plane::Update()
{
}

void Plane::Draw()
{
	ID3D11DeviceContext* devcontext = GetDX11DeviceContext();
	// 頂点バッファをセットする
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	devcontext->IASetVertexBuffers(0, 1, mpVertexBuffer.GetAddressOf(), &stride, &offset);

	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);		// トポロジーをセット（旧プリミティブタイプ）
	devcontext->IASetInputLayout(mpVertexLayout.Get());								// 頂点レイアウトセット
	devcontext->VSSetShader(mpVertexShader.Get(), nullptr, 0);						// 頂点シェーダーをセット
	devcontext->PSSetShader(mpPixelShader.Get(), nullptr, 0);						// ピクセルシェーダーをセット

	if (mDrawPosList.empty())
	{
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eWorld, mTransform.worldMtx);
		// テクスチャセット
		devcontext->PSSetShaderResources(0, 1, mTexInfo[mTexInfoNum].texSrv.GetAddressOf());
		devcontext->Draw(4, 0);
		return;
	}

	for (Float3 pos : mDrawPosList)
	{
		mTransform.position = pos;
		// ワールド変換行列セット
		mTransform.CreateWordMtx();
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eWorld, mTransform.worldMtx);
		// テクスチャセット
		devcontext->PSSetShaderResources(0, 1, mTexInfo[mTexInfoNum].texSrv.GetAddressOf());
		devcontext->Draw(4, 0);
	}

	mDrawPosList.clear();
}

void Plane::Uninit()
{

}