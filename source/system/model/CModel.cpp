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

	// ���_�f�[�^�̒�`�i�A�j���[�V�����Ή��j
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	unsigned int numElements = ARRAYSIZE(layout);

	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
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

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
							device,		// �f�o�C�X�I�u�W�F�N�g
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

	// ���_�V�F�[�_�[���
	if (mpVertexShader)
	{
		mpVertexShader->Release();
		mpVertexShader = nullptr;
	}

	// �s�N�Z���V�F�[�_�[���
	if (mpPixelShader)
	{
		mpPixelShader->Release();
		mpPixelShader = nullptr;
	}

	// ���_���C�A�E�g���
	if (mpVertexLayout)
	{
		mpVertexLayout->Release();
		mpVertexLayout = nullptr;
	}
}

// �A�j���[�V�����X�V
void CModel::Update(
	unsigned int _animno,
	const XMFLOAT4X4& _mtxworld) {

}

void CModel::Draw(XMFLOAT4X4& _mtxworld)
{
	ID3D11DeviceContext*	devcontext;			// �f�o�C�X�R���e�L�X�g
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	// ���_�t�H�[�}�b�g���Z�b�g
	devcontext->IASetInputLayout(mpVertexLayout);
	// ���_�V�F�[�_�[���Z�b�g
	devcontext->VSSetShader(mpVertexShader, nullptr, 0);
	// �s�N�Z���V�F�[�_�[���Z�b�g
	devcontext->PSSetShader(mpPixelShader, nullptr, 0);
	// �`��
	mAssimpFile.Draw(devcontext, _mtxworld);
}

bool CModel::SetPsShader(const char* _psfile)
{
	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// �s�N�Z���V�F�[�_�[�𐶐�
	bool sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
								 device,		// �f�o�C�X�I�u�W�F�N�g
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