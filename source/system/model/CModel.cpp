#include	<windows.h>
#include	"CModel.h"
#include	"../dx11/Shader.h"
#include	"../dx11/CDirectxGraphics.h"
#include	"../dx11/DX11Settransform.h"

bool CModel::Init(const char* _fileName, const char* _vsFile, const char* psFile, std::string _texFolder) {

	bool sts;

	std::string fname(_fileName);

	sts = mAssimpFile.Load(_texFolder, fname);
	if (!sts)
	{
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", _fileName);
		MessageBox(nullptr, str, "error", MB_OK);
		return false;
	}
	mFileType = eASSIMPFILE;

	// 頂点データの定義（アニメーション対応）
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	unsigned int numElements = ARRAYSIZE(layout);

	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	sts = CreateVertexShader(device,
							 _vsFile,
							 "main",
							 "vs_5_0",
							 layout,
							 numElements,
							 &mpVertexShader,
							 &mpVertexLayout);
	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return false;
	}

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
							device,		// デバイスオブジェクト
							psFile,
							"main",
							"ps_5_0",
							&mpPixelShader);
	if (!sts)
	{
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return false;
	}

	return true;
}

void CModel::Uninit() {

	// 頂点シェーダー解放
	if (mpVertexShader)
	{
		mpVertexShader->Release();
		mpVertexShader = nullptr;
	}

	// ピクセルシェーダー解放
	if (mpPixelShader)
	{
		mpPixelShader->Release();
		mpPixelShader = nullptr;
	}

	// 頂点レイアウト解放
	if (mpVertexLayout)
	{
		mpVertexLayout->Release();
		mpVertexLayout = nullptr;
	}
}

// アニメーション更新
void CModel::Update(
	unsigned int _animno,
	const XMFLOAT4X4& _mtxworld) {

}

void CModel::Draw(XMFLOAT4X4& _mtxworld)
{
	ID3D11DeviceContext*	devcontext;			// デバイスコンテキスト
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	// 頂点フォーマットをセット
	devcontext->IASetInputLayout(mpVertexLayout);
	// 頂点シェーダーをセット
	devcontext->VSSetShader(mpVertexShader, nullptr, 0);
	// ピクセルシェーダーをセット
	devcontext->PSSetShader(mpPixelShader, nullptr, 0);
	// 描画
	mAssimpFile.Draw(devcontext, _mtxworld);
}

bool CModel::SetPsShader(const char* _psfile)
{
	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// ピクセルシェーダーを生成
	bool sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
								 device,		// デバイスオブジェクト
								 _psfile,
								 "main",
								 "ps_5_0",
								 &mpPixelShader);
	if (!sts)
	{
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return false;
	}
	return true;
}