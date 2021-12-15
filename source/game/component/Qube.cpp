#include	"Qube.h"
#include	"../../system/dx11/dx11mathutil.h"
#include	"../../system/dx11/DX11util.h"
#include	"../../system/dx11/DX11Settransform.h"
#include	"../../system/dx11/Shader.h"

// �@���x�N�g�����v�Z
// �m�[�}���C�Y
void Qube::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal) {
	DX11Vec3Normalize(Normal, Normal);
}

// �C���f�b�N�X�f�[�^���쐬
void Qube::CreateIndex() {

	int vsidx[36] = { 2, 0, 1, 1, 3, 2, 4, 0, 2, 2, 6, 4,5, 1, 0, 0, 4, 5, 7, 3, 1, 1, 5, 7, 6, 2, 3, 3, 7, 6, 4, 6, 7, 7, 5, 4 };

	// �C���f�b�N�X����
	for (unsigned int i = 0; i < 12; i++)
	{
		Face f;
		// �㔼��
		f.idx[0] = vsidx[i * 3];
		f.idx[1] = vsidx[i * 3 + 1];
		f.idx[2] = vsidx[i * 3 + 2];
		mFace.emplace_back(f);
	}
}

// ���_�f�[�^���쐬
void Qube::CreateVertex() {

	// �e���_��ݒ�
	mVertex[0].Pos = { -mAabb.hl.x, mAabb.hl.y, mAabb.hl.z };
	mVertex[1].Pos = { mAabb.hl.x, mAabb.hl.y,mAabb.hl.z };
	mVertex[2].Pos = { -mAabb.hl.x, mAabb.hl.y, -mAabb.hl.z };
	mVertex[3].Pos = { mAabb.hl.x, mAabb.hl.y, -mAabb.hl.z };
	mVertex[4].Pos = { -mAabb.hl.x, -mAabb.hl.y, mAabb.hl.z };
	mVertex[5].Pos = { mAabb.hl.x, -mAabb.hl.y, mAabb.hl.z };
	mVertex[6].Pos = { -mAabb.hl.x, -mAabb.hl.y, -mAabb.hl.z };
	mVertex[7].Pos = { mAabb.hl.x, -mAabb.hl.y, -mAabb.hl.z };

	for (int i = 0; i < 8; i++)
	{
		mVertex[i].Normal = mAabb.p;			// �@�����Z�b�g
		mVertex[i].Color = mColor;				// ���_�J���[
	}
}

void Qube::SetColor(const XMFLOAT4& _color)
{
	mColor = _color;
	if (mVertex.empty())
		return;
	for (auto &v : mVertex)
	{
		v.Color = mColor;				// ���_�J���[
	}
	ID3D11Device* device = GetDX11Device();
	// ���_�o�b�t�@�쐬
	bool sts = CreateVertexBufferWrite(
		device,
		sizeof(Vertex),						// �P���_������o�C�g��
		static_cast<unsigned int>(mVertex.size()),					// ���_��
		mVertex.data(),					// ���_�f�[�^�i�[�������擪�A�h���X
		mpVertexBuffer.GetAddressOf());	// ���_�o�b�t�@
	if (!sts)
	{
		MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
	}

}

bool Qube::Init(const Primitive::AABB& _aabb, const XMFLOAT4& color) {
	ID3D11Device* device = GetDX11Device();
	bool sts;

	// ���_�f�[�^���쐬
	for (int i = 0; i < 8; i++)
	{
		Vertex v;
		mVertex.emplace_back(v);
	}
	mAabb = _aabb;

	// �J���[�l��ۑ�
	mColor = color;

	// ���̃C���f�b�N�X�f�[�^���쐬
	CreateIndex();

	// ���̒��_�f�[�^���쐬
	CreateVertex();

	// ���_�o�b�t�@�쐬
	sts = CreateVertexBufferWrite(
		device,
		sizeof(Vertex),						// �P���_������o�C�g��
		static_cast<unsigned int>(mVertex.size()),					// ���_��
		mVertex.data(),					// ���_�f�[�^�i�[�������擪�A�h���X
		mpVertexBuffer.GetAddressOf());	// ���_�o�b�t�@
	if (!sts)
	{
		MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
		return false;
	}

	// �C���f�b�N�X�o�b�t�@�쐬
	sts = CreateIndexBuffer(
		device,								// �f�o�C�X�I�u�W�F�N�g
		static_cast<unsigned int>(mFace.size()) * 3,					// �C���f�b�N�X��
		mFace.data(),						// �C���f�b�N�X�f�[�^�擪�A�h���X
		mpIndexBuffer.GetAddressOf());		// �C���f�b�N�X�o�b�t�@

	if (!sts)
	{
		MessageBox(NULL, "CreateBuffer(index buffer) error", "Error", MB_OK);
		return false;
	}

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	sts = CreateVertexShader(device,
							 "shader/vssphere.hlsl",
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
							device,							// �f�o�C�X�I�u�W�F�N�g
							"shader/pssphere.hlsl",
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

void Qube::Update(const Primitive::AABB& _aabb)
{
	mAabb = _aabb;
	CreateVertex();
}

// �`��
void Qube::Draw(DirectX::XMFLOAT4X4 _mtx) {
	ID3D11DeviceContext* deviceContext = GetDX11DeviceContext();

	// ���_�o�b�t�@���Z�b�g����
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, mpVertexBuffer.GetAddressOf(), &stride, &offset);

	deviceContext->IASetIndexBuffer(mpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);			// �C���f�b�N�X�o�b�t�@���Z�b�g
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
	deviceContext->IASetInputLayout(mpVertexLayout.Get());					// ���_���C�A�E�g�Z�b�g

	deviceContext->VSSetShader(mpVertexShader.Get(), nullptr, 0);			// ���_�V�F�[�_�[���Z�b�g
	deviceContext->PSSetShader(mpPixelShader.Get(), nullptr, 0);			// �s�N�Z���V�F�[�_�[���Z�b�g

	// ���[���h�ϊ��s��Z�b�g
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eWorld, _mtx);

	deviceContext->DrawIndexed(
		static_cast<unsigned int>(mFace.size() * 3),		// �`�悷��C���f�b�N�X���i�ʐ��~�R�j
		0,									// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
		0);									// ���_�o�b�t�@�̍ŏ�����g��
}

