#include	"texture_manager.h"
#include	"../dx11/DX11Settransform.h"
#include	<memory>
#include	<wrl/client.h>
#include	"shaderhashmap.h"

// 使用する頂点シェーダー名
const char* vsfilename[] = {
	"shader/basicvs.hlsl"
};
// 使用するピクセルシェーダー名
const char* psfilename[] = {
	"shader/basicps.hlsl",
	"shader/basicnotexps.hlsl"
};

void TextureManager::Finalize()
{
}

TextureManager::TextureManager()
{
	mpDevice = CDirectXGraphics::GetInstance()->GetDXDevice();
	// ピクセルシェーダーを生成
	bool sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[0]);
	if (!sts)
	{
		MessageBox(nullptr, "SetPixelShader error(basicps.hlsl)", "error", MB_OK);
		return;
	}

	// ピクセルシェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[1]);
	if (!sts)
	{
		MessageBox(nullptr, "SetPixelShader error(basicnotexps.hlsl)", "error", MB_OK);
		return;
	}

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// エレメント数
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		vsfilename[0],			// 頂点シェーダーファイル名
		layout,					// 頂点レイアウト
		numElements);			// エレメント数
}

bool TextureManager::LoadTexture(std::string _texFileName)
{
	// 存在するか確かめる
	auto it = mTextureInfo.find(_texFileName);

	// 存在していれば
	if (it != mTextureInfo.end())
		return false;

	mTextureInfo[_texFileName];

	// テクスチャ設定
	ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	bool sts = CreateSRVfromFile(_texFileName.c_str(), mpDevice, devicecontext, &mTextureInfo[_texFileName].texRes, &mTextureInfo[_texFileName].texSrv);
	if (!sts)
	{
		MessageBox(nullptr, "CreateSRVfromfile エラー", "error!!", MB_OK);
		return false;
	}
	return true;
}

void TextureManager::Draw(TextureInfo _info, DirectX::XMFLOAT4X4 _mtx)
{
	// デバイスコンテキストを取得
	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// 頂点シェーダー、ピクセルシェーダー取得
	ID3D11VertexShader* vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
	ID3D11PixelShader* ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

	// 頂点レイアウト取得
	ID3D11InputLayout* layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);

	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// 頂点フォーマットをセット
	devcontext->IASetInputLayout(layout);

	// ドローコール発行
	devcontext->DrawIndexed(
		4,						// インデックス数
		0,						// 開始インデックス
		0);						// 基準頂点インデックス

}

TextureInfo* TextureManager::GetTexturePtr(std::string _key)
{
	// 存在するかを確かめる
	auto it = mTextureInfo.find(_key);
	if (it != mTextureInfo.end())
	{
		return &mTextureInfo.at(_key);
	}
	// 存在しなければ読み込む
	if (LoadTexture(_key))
	{
		it = mTextureInfo.find(_key);
		if (it != mTextureInfo.end())
		{
			return &mTextureInfo.at(_key);
		}
	}
	return nullptr;
}
