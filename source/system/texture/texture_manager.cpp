#include	"texture_manager.h"
#include	"../dx11/DX11Settransform.h"

void TextureManager::Finalize()
{
}

bool TextureManager::LoadTexture(const std::string& _texFileName)
{
	// 存在するかを確かめる
	auto it = mTextureInfo.find(_texFileName);

	// 存在していれば
	if (it != mTextureInfo.end())
		return;

	mTextureInfo[_texFileName];

	// デバイスを取得
	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// テクスチャ設定
	device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	bool sts = CreateSRVfromFile(_texFileName.c_str, device, devicecontext, &mTextureInfo[_texFileName].texRes, &mTextureInfo[_texFileName].texSrv);
	if (!sts)
	{
		MessageBox(nullptr, "CreateSRVfromfile エラー", "error!!", MB_OK);
		return false;
	}
	return false;
}

TextureInfo* TextureManager::GetTexturePtr(std::string _key)
{
	// 存在するかを確かめる
	auto it = mTextureInfo.find(_key);
	if (it == mTextureInfo.end())
	{
		return nullptr;
	}
	return &mTextureInfo.at(_key);
}
