#include	"plane.h"
#include	"../../system/dx11/DX11util.h"
#include	"../../system/dx11/DX11Settransform.h"
#include	"../../system/dx11/Shader.h"

void Plane::ObjectInit()
{
	ID3D11Device* dev;
	ID3D11DeviceContext* devcontext;

	dev = GetDX11Device();
	devcontext = GetDX11DeviceContext();

	// �e�N�X�`���ǂݍ���
	for (int i = 0; i < 4; i++)
	{
		mTexInfo[i] = *TextureManager::GetInstance().GetTexturePtr(mTexFileName[i]);
	}

	bool sts = CreateVertexBuffer(dev, sizeof(Vertex), 4, mVartex, &mpVertexBuffer);
	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexBuffer error(stage)", "error", MB_OK);
	}

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	sts = CreateVertexShader(dev, "shader/vs.hlsl", "main", "vs_5_0", layout, numElements, &mpVertexShader, &mpVertexLayout);
	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexShader error(stage)", "error", MB_OK);
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = CreatePixelShader(dev, "shader/pstexcol.hlsl", "main", "ps_5_0", &mpPixelShader);
	if (!sts)
	{
		MessageBox(nullptr, "CreatePixelShader error(stage)", "error", MB_OK);
	}
	// ���s�ړ��ʂ��v�Z

	DX11MtxIdentity(mTransform.worldMtx);	// �P�ʍs��
	mTransform.CreateMtx();
}

void Plane::ObjectUpdate()
{
}

void Plane::ObjectDraw()
{
	ID3D11DeviceContext* devcontext = GetDX11DeviceContext();
	// ���_�o�b�t�@���Z�b�g����
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	devcontext->IASetVertexBuffers(0, 1, mpVertexBuffer.GetAddressOf(), &stride, &offset);

	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
	devcontext->IASetInputLayout(mpVertexLayout.Get());								// ���_���C�A�E�g�Z�b�g
	devcontext->VSSetShader(mpVertexShader.Get(), nullptr, 0);						// ���_�V�F�[�_�[���Z�b�g
	devcontext->PSSetShader(mpPixelShader.Get(), nullptr, 0);						// �s�N�Z���V�F�[�_�[���Z�b�g

	// ���[���h�ϊ��s��Z�b�g
	mTransform.CreateMtx();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mTransform.worldMtx);

	// �e�N�X�`���Z�b�g
	/// ���̃e�N�X�`���̔z��̓Y������ς��邱�Ƃŉ摜��ύX�ł���
	devcontext->PSSetShaderResources(0, 1, mTexInfo[1].texSrv.GetAddressOf());
	devcontext->Draw(4, 0);
}

void Plane::Uninit()
{

}