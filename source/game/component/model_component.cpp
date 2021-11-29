#include	"model_component.h"
#include	"../../system/dx11/Shader.h"
#include	"../../system/dx11/CDirectxGraphics.h"
#include	"../../system/dx11/DX11Settransform.h"

#include	<windows.h>

using namespace Component;

//void ModelComponent::SetModelData(const char * _fileName, const char * _vsFile, const char * _psFile, std::string _texFolder)
//{
//bool sts;
//
//std::string fname(_fileName);
//
//sts = mAssimpFile.Load(_texFolder, fname);
//if (!sts)
//{
//	char str[128];
//	sprintf_s(str, 128, "%s load ERROR!!", _fileName);
//	MessageBox(nullptr, str, "error", MB_OK);
//	return;
//}
//mFileType = eASSIMPFILE;
//
//// 頂点データの定義（アニメーション対応）
//D3D11_INPUT_ELEMENT_DESC layout[] =
//{
//	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//};
//unsigned int numElements = ARRAYSIZE(layout);
//
//ID3D11Device* device;
//device = CDirectXGraphics::GetInstance()->GetDXDevice();
//
//// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
//sts = CreateVertexShader(device,
//						 _vsFile,
//						 "main",
//						 "vs_5_0",
//						 layout,
//						 numElements,
//						 &mpVertexShader,
//						 &mpVertexLayout);
//if (!sts)
//{
//	MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
//	return;
//}
//
//// ピクセルシェーダーを生成
//sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
//						device,		// デバイスオブジェクト
//						_psFile,
//						"main",
//						"ps_5_0",
//						&mpPixelShader);
//if (!sts)
//{
//	MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
//	return;
//}
//}

void ModelComponent::Draw()
{
	_mpModel->Draw(mOwner->GetTransform()->mtx);
}

void ModelComponent::Uninit()
{
}

//bool ModelComponent::SetPsShader(const char * _psfile)
//{
//	ID3D11Device* device;
//	device = CDirectXGraphics::GetInstance()->GetDXDevice();
//
//	// ピクセルシェーダーを生成
//	bool sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
//								 device,		// デバイスオブジェクト
//								 _psfile,
//								 "main",
//								 "ps_5_0",
//								 &mpPixelShader);
//	if (!sts)
//	{
//		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
//		return false;
//	}
//	return true;
//}