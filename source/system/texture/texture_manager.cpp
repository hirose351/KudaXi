#include	"texture_manager.h"
#include	"../dx11/DX11Settransform.h"

void TextureManager::Finalize()
{
}

bool TextureManager::LoadTexture(const std::string& _texFileName)
{
	// ���݂��邩���m���߂�
	auto it = mTextureInfo.find(_texFileName);

	// ���݂��Ă����
	if (it != mTextureInfo.end())
		return;

	mTextureInfo[_texFileName];

	// �f�o�C�X���擾
	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// �e�N�X�`���ݒ�
	device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	bool sts = CreateSRVfromFile(_texFileName.c_str, device, devicecontext, &mTextureInfo[_texFileName].texRes, &mTextureInfo[_texFileName].texSrv);
	if (!sts)
	{
		MessageBox(nullptr, "CreateSRVfromfile �G���[", "error!!", MB_OK);
		return false;
	}
	return false;
}

TextureInfo* TextureManager::GetTexturePtr(std::string _key)
{
	// ���݂��邩���m���߂�
	auto it = mTextureInfo.find(_key);
	if (it == mTextureInfo.end())
	{
		return nullptr;
	}
	return &mTextureInfo.at(_key);
}
