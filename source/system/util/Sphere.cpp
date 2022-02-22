#include	"sphere.h"
#include	"../dx11/dx11_mathutil.h"

// �@���x�N�g�����v�Z,�m�[�}���C�Y
void Sphere::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal)
{
	DX11Vec3Normalize(Normal, Normal);
}

// �C���f�b�N�X�f�[�^���쐬
void Sphere::CreateIndex()
{
	// �C���f�b�N�X����
	for (unsigned int y = 0; y < mDivY; y++)
	{
		for (unsigned int x = 0; x < mDivX; x++)
		{
			int count = (mDivX + 1)*y + x;			// ������W�̃C���f�b�N�X

			Face f;
			// �㔼��
			f.idx[0] = count;
			f.idx[1] = count + 1;
			f.idx[2] = count + 1 + (mDivX + 1);

			mFace.emplace_back(f);

			// ������
			f.idx[0] = count;
			f.idx[1] = count + (mDivX + 1) + 1;
			f.idx[2] = count + (mDivX + 1);

			mFace.emplace_back(f);

		}
	}
}

// ���_�f�[�^���쐬
void Sphere::CreateVertex()
{
	float azimuth = 0.0f;			// ���ʊp
	float elevation = 0.0f;			// �p

	XMFLOAT3	Normal;
	// ���ʊp�Ƌp���狅���b�V���̒��_�f�[�^���쐬
	for (unsigned int y = 0; y <= mDivY; y++)
	{
		elevation = (PI * (float)y) / (float)mDivY;			// �p���Z�b�g
		float r = mRadius * sinf(elevation);				// �p�ɉ��������a���v�Z

		for (unsigned int x = 0; x <= mDivX; x++)
		{
			azimuth = (2 * PI * (float)x) / (float)mDivX;	// ���ʊp���Z�b�g

			Vertex v;
			v.Pos.x = r * cosf(azimuth);
			v.Pos.y = mRadius * cosf(elevation);
			v.Pos.z = r * sinf(azimuth);

			Normalize(v.Pos, Normal);		// �@�����v�Z
			v.Normal = Normal;				// �@�����Z�b�g

			v.Color = mColor;				// ���_�J���[

			mVertex.emplace_back(v);
		}
	}
}

// �`��
void Sphere::Draw(ID3D11DeviceContext* _device)
{
	// ���_�o�b�t�@���Z�b�g����
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	_device->IASetVertexBuffers(0, 1, mpVertexBuffer.GetAddressOf(), &stride, &offset);

	_device->IASetIndexBuffer(mpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);	// �C���f�b�N�X�o�b�t�@���Z�b�g
	_device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
	_device->IASetInputLayout(mpVertexLayout.Get());							// ���_���C�A�E�g�Z�b�g

	_device->VSSetShader(mpVertexShader.Get(), nullptr, 0);			// ���_�V�F�[�_�[���Z�b�g
	_device->PSSetShader(mpPixelShader.Get(), nullptr, 0);			// �s�N�Z���V�F�[�_�[���Z�b�g

	_device->DrawIndexed(
		static_cast<unsigned int>(mFace.size() * 3),		// �`�悷��C���f�b�N�X���i�ʐ��~�R�j
		0,													// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
		0);													// ���_�o�b�t�@�̍ŏ�����g��
}

bool Sphere::Init(float _r,					// ���a
				  int _divisionHorizontal,	// ���������̕�����
				  int _divisionVertical,	// ���������̕�����
				  XMFLOAT4 color,
				  ID3D11Device* device)
{
	bool sts;
	// ��������ۑ�
	mDivX = _divisionHorizontal;
	mDivY = _divisionVertical;

	// ���a��ۑ�
	mRadius = _r;

	// �J���[�l��ۑ�
	mColor = color;

	// ���̃C���f�b�N�X�f�[�^���쐬
	CreateIndex();

	// ���̒��_�f�[�^���쐬
	CreateVertex();

	// ���_�o�b�t�@�쐬
	sts = CreateVertexBufferWrite(
		device,
		sizeof(Vertex),								// �P���_������o�C�g��
		static_cast<unsigned int>(mVertex.size()),	// ���_��
		mVertex.data(),								// ���_�f�[�^�i�[�������擪�A�h���X
		mpVertexBuffer.GetAddressOf());				// ���_�o�b�t�@
	if (!sts)
	{
		MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
		return false;
	}

	// �C���f�b�N�X�o�b�t�@�쐬
	sts = CreateIndexBuffer(
		device,											// �f�o�C�X�I�u�W�F�N�g
		static_cast<unsigned int>(mFace.size()) * 3,	// �C���f�b�N�X��
		mFace.data(),									// �C���f�b�N�X�f�[�^�擪�A�h���X
		mpIndexBuffer.GetAddressOf());					// �C���f�b�N�X�o�b�t�@

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
	sts = CreatePixelShader(						// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
							device,					// �f�o�C�X�I�u�W�F�N�g
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
