#include	"CBillboard.h"
#include	"../dx11/CDirectxGraphics.h"
#include	"../dx11/Shader.h"
#include	"../dx11/DX11Settransform.h"
#include	"../dx11/dx11mathutil.h"
#include	"../../game/gameobject/CCamera.h"

ComPtr <ID3D11InputLayout> CBillboard::smIL = nullptr;
ComPtr <ID3D11VertexShader> CBillboard::smVS = nullptr;
ComPtr <ID3D11PixelShader> CBillboard::smPS = nullptr;
std::unordered_map<std::string, CBillboard::TextureInfo> CBillboard::smTextureInfo;

CBillboard::CBillboard() :
	mScale({ 1.f,1.f }), mColor({ 1.f,1.f,1.f,1.f }),
	mUV0({ 0.f,0.f }), mUV3({ 1.f,1.f })
{
	static bool ShouldInit = false;

	if (!ShouldInit)
	{
		CBillboard::CreateShader();
		ShouldInit = true;
	}

	Initialize();
}

HRESULT CBillboard::Initialize(void)
{
	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();

	XMFLOAT3 v[4] = {
	{-0.5f ,0.5f, 0.0f },
	{0.5f, 0.5f, 0.0f },
	{-0.5f, -0.5f, 0.0f},
	{0.5f, -0.5f, 0.0f}
	};

	// 頂点バッファを生成
	bool sts = CreateVertexBuffer(
		device,						// デバイスオブジェクト
		sizeof(XMFLOAT3),			// １頂点当たりバイト数
		4,							// 頂点数
		&v,							// 頂点データ格納メモリ先頭アドレス
		&mpVetexBuffer				// 頂点バッファ
	);

	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return E_FAIL;
	}

	// 定数バッファを生成
	sts = CreateConstantBufferWrite(device, sizeof(CB_DATA), &mpConstantBuffer);
	if (!sts)
	{
		MessageBox(nullptr, "Create Constant Buffer Error", "error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CBillboard::LoadTexture(const std::string pTexFileName)
{
	if (smTextureInfo.count(pTexFileName) == 0)
	{
		smTextureInfo[pTexFileName];

		bool sts = CreateSRVfromFile(pTexFileName.c_str(),
									 CDirectXGraphics::GetInstance()->GetDXDevice(),
									 CDirectXGraphics::GetInstance()->GetImmediateContext(),
									 &smTextureInfo.at(pTexFileName).texRes,
									 &smTextureInfo.at(pTexFileName).texSrv);
		if (!sts)
		{
			MessageBox(nullptr, "Texture Load Error", "error", MB_OK);
			return E_FAIL;
		}
	}

	mpTexRes = smTextureInfo.at(pTexFileName).texRes;
	mpTexSRV = smTextureInfo.at(pTexFileName).texSrv;

	return S_OK;
}

void CBillboard::Update(void)
{
	XMFLOAT4X4 matrixRot;

	XMStoreFloat4x4(&mMatrixWorld, XMMatrixIdentity());
	matrixRot = CCamera::GetInstance()->GetCameraMatrix();
	XMStoreFloat4x4(&matrixRot, XMMatrixInverse(nullptr, XMLoadFloat4x4(&matrixRot)));

	mMatrixWorld._11 = mScale.x * matrixRot._11;
	mMatrixWorld._12 = mScale.x * matrixRot._12;
	mMatrixWorld._13 = mScale.x * matrixRot._13;

	mMatrixWorld._21 = mScale.y * matrixRot._21;
	mMatrixWorld._22 = mScale.y * matrixRot._22;
	mMatrixWorld._23 = mScale.y * matrixRot._23;

	mMatrixWorld._31 = matrixRot._31;
	mMatrixWorld._32 = matrixRot._32;
	mMatrixWorld._33 = matrixRot._33;

	mMatrixWorld._41 = mPos.x;
	mMatrixWorld._42 = mPos.y;
	mMatrixWorld._43 = mPos.z;
}

void CBillboard::Render(void)
{
	ID3D11DeviceContext*		 devicecontext;				// デバイスコンテキスト

	// デバイスコンテキストを取得する
	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	unsigned int stride = sizeof(XMFLOAT3);					// ストライドをセット（１頂点当たりのバイト数）
	unsigned  offset = 0;									// オフセット値をセット

	// 頂点バッファをデバイスコンテキストへセット
	devicecontext->IASetVertexBuffers(
		0,													// スタートスロット
		1,													// 頂点バッファ個数
		mpVetexBuffer.GetAddressOf(),									// 頂点バッファの先頭アドレス
		&stride,											// ストライド
		&offset);											// オフセット

	// トポロジーをセット
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点フォーマットをセット
	devicecontext->IASetInputLayout(smIL.Get());

	devicecontext->VSSetShader(smVS.Get(), nullptr, 0);		// 頂点シェーダーをセット
	devicecontext->PSSetShader(smPS.Get(), nullptr, 0);		// ピクセルシェーダーをセット

	if (mpTexSRV != nullptr)devicecontext->PSSetShaderResources(0, 1, mpTexSRV.GetAddressOf());  // シェーダーがテクスチャにアクセスできるようにセット

	CB_DATA cbdata;

	cbdata.r = mColor.x;
	cbdata.g = mColor.y;
	cbdata.b = mColor.z;
	cbdata.a = mColor.w;

	cbdata.u0 = mUV0.x;
	cbdata.v0 = mUV0.y;
	cbdata.u3 = mUV3.x;
	cbdata.v3 = mUV3.y;

	D3D11_MAPPED_SUBRESOURCE mapRes;
	HRESULT hr = devicecontext->Map(mpConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);

	if (SUCCEEDED(hr))
	{
		memcpy_s(mapRes.pData, mapRes.RowPitch, &cbdata, sizeof(cbdata));
		devicecontext->Unmap(mpConstantBuffer.Get(), 0);
	}

	devicecontext->VSSetConstantBuffers(3, 1, mpConstantBuffer.GetAddressOf());
	devicecontext->PSSetConstantBuffers(3, 1, mpConstantBuffer.GetAddressOf());

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eWorld, mMatrixWorld);

	// デバイスコンテキストに頂点データを流し込む
	devicecontext->Draw(
		4,									// 頂点数
		0);									// 開始頂点インデックス
}

HRESULT CBillboard::CreateShader(void)
{
	// デバイスを取得する
	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// 頂点（レイアウト）データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "SV_POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// エレメント数
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	bool sts = CreateVertexShader(
		device,								// デバイスオブジェクト						
		"shader/Billboard.hlsl", // 頂点シェーダーソースファイル					
		"vs_main",								// エントリ関数
		"vs_5_0",							// 頂点シェーダーモデル５．０
		layout,								// 頂点データレイアウト
		numElements,						// 頂点データのエレメント数
		&smVS,					// 頂点シェーダーオブジェクト
		&smIL);					// 頂点データレイアウトオブジェクト

	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return E_FAIL;
	}

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(							// ピクセルシェーダーオブジェクトを生成
							device,						// デバイスオブジェクト
							"shader/ps2d.hlsl",			// ピクセルシェーダーソースファイル
							"ps_main",					// エントリ関数
							"ps_5_0",					// ピクセルシェーダーモデル５．０
							&smPS);						// ピクセルシェーダーオブジェクト

	if (!sts)
	{
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CBillboard::SetRotation(XMFLOAT4X4 _mtx)
{
	DX11MtxMultiply(mMatrixWorld, mMatrixWorld, _mtx);
}

