#include	"texture_manager.h"
#include	"../dx11/DX11Settransform.h"
#include	<memory>
#include	<wrl/client.h>
#include	"shaderhashmap.h"

// �g�p���钸�_�V�F�[�_�[��
const char* vsfilename[] = {
	"shader/basicvs.hlsl"
};
// �g�p����s�N�Z���V�F�[�_�[��
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
	// �s�N�Z���V�F�[�_�[�𐶐�
	bool sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[0]);
	if (!sts)
	{
		MessageBox(nullptr, "SetPixelShader error(basicps.hlsl)", "error", MB_OK);
		return;
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[1]);
	if (!sts)
	{
		MessageBox(nullptr, "SetPixelShader error(basicnotexps.hlsl)", "error", MB_OK);
		return;
	}

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// �G�������g��
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		vsfilename[0],			// ���_�V�F�[�_�[�t�@�C����
		layout,					// ���_���C�A�E�g
		numElements);			// �G�������g��
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

void TextureManager::Draw(TextureInfo _info, DirectX::XMFLOAT4X4 _mtx)
{
	// �f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�擾
	ID3D11VertexShader* vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
	ID3D11PixelShader* ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

	// ���_���C�A�E�g�擾
	ID3D11InputLayout* layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);

	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// ���_�t�H�[�}�b�g���Z�b�g
	devcontext->IASetInputLayout(layout);

	// �h���[�R�[�����s
	devcontext->DrawIndexed(
		4,						// �C���f�b�N�X��
		0,						// �J�n�C���f�b�N�X
		0);						// ����_�C���f�b�N�X

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
