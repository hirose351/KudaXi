#include	"quad2d.h"
#include	"../dx11/dx11_settransform.h"

// comptr
using Microsoft::WRL::ComPtr;

// �g�p���钸�_�V�F�[�_�[��
const char* vsfilename[] = {
	"shader/basicvs.hlsl"
};
// �g�p����s�N�Z���V�F�[�_�[��
const char* psfilename[] = {
	"shader/basicps.hlsl",
	"shader/basicnotexps.hlsl",
	"shader/psmokomoko.hlsl"
};

// ��`�̏�����
bool CQuad2D::Init(DirectX::XMFLOAT2 _scale, std::string _texName, const DirectX::XMFLOAT4 &_color, int _u, int _v, float _z) {

	// 4�p�`�̏���������
	XMFLOAT2 uv[4] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{0.0f,1.0f},
		{1.0f,1.0f}
	};

	mScale = _scale;
	mColor = _color;

	// �s�N�Z���V�F�[�_�[�𐶐�
	bool sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[0]);
	if (!sts)
	{
		MessageBox(nullptr, "SetPixelShader error(basicps.hlsl)", "error", MB_OK);
		return false;
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[1]);
	if (!sts)
	{
		MessageBox(nullptr, "SetPixelShader error(basicnotexps.hlsl)", "error", MB_OK);
		return false;
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[2]);
	if (!sts)
	{
		MessageBox(nullptr, "SetPixelShader error(psmokomoko.hlsl)", "error", MB_OK);
		return false;
	}

	// �R���X�^���g�o�b�t�@�쐬
	sts = CreateConstantBuffer(
		GetDX11Device(),				// �f�o�C�X
		sizeof(ConstantBufferTime),		// �T�C�Y
		&mpConstantBufferTime);			// �R���X�^���g�o�b�t�@�S

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

	// ���_������
	UpdateVertex(_scale, _color, _z);

	// �f�o�C�X���擾
	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// ���_�o�b�t�@�𐶐�
	sts = CreateVertexBufferWrite(
		device,						// �f�o�C�X�I�u�W�F�N�g
		sizeof(CQuad2D::Vertex),		// �P���_������o�C�g��
		4,							// ���_��
		mVertex,					// ���_�f�[�^�i�[�������擪�A�h���X
		&mVertexbuffer				// ���_�o�b�t�@
	);
	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	// �C���f�b�N�X�o�b�t�@�𐶐�
	uint32_t index[4] = { 0,1,2,3 };

	sts = CreateIndexBuffer(
		device,						// �f�o�C�X�I�u�W�F�N�g
		4,							// �C���f�b�N�X��
		index,						// �C���f�b�N�X�i�[�������擪�A�h���X
		&mIndexbuffer				// �C���f�b�N�X�o�b�t�@
	);
	if (!sts)
	{
		MessageBox(nullptr, "CreateIndexBuffer error", "error", MB_OK);
		return false;
	}

	// �e�N�X�`���ݒ�
	if (_texName != "")
	{
		device = CDirectXGraphics::GetInstance()->GetDXDevice();
		ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

		sts = CreateSRVfromFile(_texName.c_str(), device, devicecontext, &mTexInfo.texRes, &mTexInfo.texSrv);
		if (!sts)
		{
			// �e�N�X�`���s�v�ȏꍇ��NoTex�Ɠ��͂��ĕ`�悳���Ȃ��ł���
			MessageBox(nullptr, "CreateSRVfromfile �G���[", "error!!", MB_OK);
			return false;
		}
	}


	// �A�j���[�V�����ݒ�
	mDivu = _u;				// ��������������
	mDivv = _v;				// �c������������

	SetTextureUV(0, 0);
	SetColor(mColor);
	UpdateVertex(_scale, _color);
	UpdateVbuffer();

	return true;
}

// �`��
void CQuad2D::Draw(DirectX::XMFLOAT4X4 _mtx) {

	if (mColor.w <= 0.0f)
		return;

	// �f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// ���W�ϊ��p�̍s����Z�b�g
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eWorld, _mtx);
	//DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eWorld, mWorldmtx);

	unsigned int stride = sizeof(CQuad2D::Vertex);	// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
	unsigned  offset = 0;						// �I�t�Z�b�g�l���Z�b�g

	// �f�o�C�X�R���e�L�X�g���V�F�[�_�[���\�[�X�փZ�b�g
	devcontext->PSSetShaderResources(0, 1, mTexInfo.texSrv.GetAddressOf());

	// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetVertexBuffers(
		0,									// �X�^�[�g�X���b�g
		1,									// ���_�o�b�t�@��
		mVertexbuffer.GetAddressOf(),		// ���_�o�b�t�@�̐擪�A�h���X
		&stride,							// �X�g���C�h
		&offset);							// �I�t�Z�b�g

	// �C���f�b�N�X�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetIndexBuffer(
		mIndexbuffer.Get(),				// �C���f�b�N�X�o�b�t�@
		DXGI_FORMAT_R32_UINT,				// �t�H�[�}�b�g
		0);									// �I�t�Z�b�g

	// �g�|���W�[���Z�b�g
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

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

// �`��
void CQuad2D::DrawNoTex(DirectX::XMFLOAT4X4 _mtx) {

	if (mColor.w <= 0.0f)
		return;

	// �f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// ���W�ϊ��p�̍s����Z�b�g
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eWorld, _mtx);

	unsigned int stride = sizeof(CQuad2D::Vertex);	// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
	unsigned  offset = 0;						// �I�t�Z�b�g�l���Z�b�g

	// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetVertexBuffers(
		0,									// �X�^�[�g�X���b�g
		1,									// ���_�o�b�t�@��
		mVertexbuffer.GetAddressOf(),		// ���_�o�b�t�@�̐擪�A�h���X
		&stride,							// �X�g���C�h
		&offset);							// �I�t�Z�b�g

	// �C���f�b�N�X�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetIndexBuffer(
		mIndexbuffer.Get(),				// �C���f�b�N�X�o�b�t�@
		DXGI_FORMAT_R32_UINT,				// �t�H�[�}�b�g
		0);									// �I�t�Z�b�g

	// �g�|���W�[���Z�b�g
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�擾
	ID3D11VertexShader* vs;
	vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
	ID3D11PixelShader* ps;
	ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[1]);

	// ���_���C�A�E�g�擾
	ID3D11InputLayout* layout;
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);

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

void CQuad2D::DrawMokoMokoShaderTex(DirectX::XMFLOAT4X4 _mtx)
{
	if (mColor.w <= 0.0f)
		return;

	// �f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// ���W�ϊ��p�̍s����Z�b�g
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eWorld, _mtx);

	unsigned int stride = sizeof(CQuad2D::Vertex);	// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
	unsigned  offset = 0;						// �I�t�Z�b�g�l���Z�b�g

	// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetVertexBuffers(
		0,									// �X�^�[�g�X���b�g
		1,									// ���_�o�b�t�@��
		mVertexbuffer.GetAddressOf(),		// ���_�o�b�t�@�̐擪�A�h���X
		&stride,							// �X�g���C�h
		&offset);							// �I�t�Z�b�g

	// �C���f�b�N�X�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetIndexBuffer(
		mIndexbuffer.Get(),					// �C���f�b�N�X�o�b�t�@
		DXGI_FORMAT_R32_UINT,				// �t�H�[�}�b�g
		0);									// �I�t�Z�b�g

	// �g�|���W�[���Z�b�g
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	ConstantBufferTime	cb;
	cb.time = ::timeGetTime() / 1000.0f;
	devcontext->UpdateSubresource(mpConstantBufferTime, 0, nullptr, &cb, 0, 0);

	// �R���X�^���g�o�b�t�@����6���W�X�^�փZ�b�g(�s�N�Z���V�F�[�_�[�p)
	devcontext->PSSetConstantBuffers(6, 1, &mpConstantBufferTime);

	// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�擾
	ID3D11VertexShader* vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
	ID3D11PixelShader* ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[2]);

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

void CQuad2D::SetColor(const DirectX::XMFLOAT4 &_color)
{
	mColor = _color;

	CQuad2D::Vertex	v[4] = {
		// ���W													// �J���[	// UV	
		XMFLOAT3(-mScale.x / 2.0f,	-mScale.y / 2.0f, 0.0f),	_color,		mUv[0],
		XMFLOAT3(mScale.x / 2.0f,	-mScale.y / 2.0f, 0.0f),	_color,		mUv[1],
		XMFLOAT3(-mScale.x / 2.0f,	 mScale.y / 2.0f, 0.0f),	_color,		mUv[2],
		XMFLOAT3(mScale.x / 2.0f,	 mScale.y / 2.0f, 0.0f),	_color,		mUv[3]
	};

	for (int i = 0; i < 4; i++)
	{
		mVertex[i] = v[i];
	}
}

// ���_�f�[�^�X�V
void CQuad2D::UpdateVertex(DirectX::XMFLOAT2 _scale, const DirectX::XMFLOAT4 &color, float z) {

	mScale = _scale;

	CQuad2D::Vertex	v[4] = {
		// ���W													// �J���[	// UV	
		XMFLOAT3(-mScale.x / 2.0f,	-mScale.y / 2.0f, z),	color,		mUv[0],
		XMFLOAT3(mScale.x / 2.0f,	-mScale.y / 2.0f, z),	color,		mUv[1],
		XMFLOAT3(-mScale.x / 2.0f,	 mScale.y / 2.0f, z),	color,		mUv[2],
		XMFLOAT3(mScale.x / 2.0f,	 mScale.y / 2.0f, z),	color,		mUv[3]
	};

	for (int i = 0; i < 4; i++)
	{
		mVertex[i] = v[i];
	}
}

// ���_�o�b�t�@�X�V
void CQuad2D::UpdateVbuffer() {
	D3D11_MAPPED_SUBRESOURCE pData;
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	HRESULT hr = devcontext->Map(mVertexbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr))
	{
		memcpy_s(pData.pData, pData.RowPitch, (void*)(mVertex), sizeof(CQuad2D::Vertex) * 4);
		devcontext->Unmap(mVertexbuffer.Get(), 0);
	}
}


// UV���W�ݒ�@(_u�F�����牽�Ԗڂ�  _v�F�ォ�牽�Ԗڂ��@direction�F�`�����(�O���� �P���E)�@�f�t�H���g�͉E)
void CQuad2D::SetTextureUV(int _u, int _v, int direction) {

	// ������
	if (direction == 0)
	{
		mUv[0] = { 1.0f / mDivu * (_u + 1), 1.0f / mDivv * _v };
		mUv[1] = { 1.0f / mDivu * _u,	     1.0f / mDivv * _v };
		mUv[2] = { 1.0f / mDivu * (_u + 1), 1.0f / mDivv * (_v + 1) };
		mUv[3] = { 1.0f / mDivu * _u,	     1.0f / mDivv * (_v + 1) };
	}
	// �E����
	else
	{
		mUv[0] = { 1.0f / mDivu * _u,	     1.0f / mDivv * _v };
		mUv[1] = { 1.0f / mDivu * (_u + 1), 1.0f / mDivv * _v };
		mUv[2] = { 1.0f / mDivu * _u,	     1.0f / mDivv * (_v + 1) };
		mUv[3] = { 1.0f / mDivu * (_u + 1), 1.0f / mDivv * (_v + 1) };
	}
	UpdateVertex(mScale, mColor);
	UpdateVbuffer();
}