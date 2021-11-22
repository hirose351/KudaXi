#include	"texture_manager.h"
#include	"../dx11/DX11Settransform.h"
#include	<memory>
#include	<wrl/client.h>

void TextureManager::Finalize()
{
}

bool TextureManager::LoadTexture(std::string _texFileName)
{
	// ���݂��邩�m���߂�
	auto it = mTextureInfo.find(_texFileName);

	// ���݂��Ă����
	if (it != mTextureInfo.end())
		return false;

	mTextureInfo[_texFileName];

	// �e�N�X�`���ݒ�
	ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	bool sts = CreateSRVfromFile(_texFileName.c_str(), mpDevice, devicecontext, &mTextureInfo[_texFileName].texRes, &mTextureInfo[_texFileName].texSrv);
	if (!sts)
	{
		MessageBox(nullptr, "CreateSRVfromfile �G���[", "error!!", MB_OK);
		return false;
	}
	return true;
}

TextureInfo* TextureManager::GetTexturePtr(std::string _key)
{
	// ���݂��邩���m���߂�
	auto it = mTextureInfo.find(_key);
	if (it != mTextureInfo.end())
	{
		return &mTextureInfo.at(_key);
	}
	// ���݂��Ȃ���Γǂݍ���
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
