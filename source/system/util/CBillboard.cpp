#include	"CBillboard.h"
#include	"../dx11/CDirectxGraphics.h"
#include	"../dx11/Shader.h"
#include	"../dx11/DX11Settransform.h"
#include	"../dx11/dx11mathutil.h"
#include	"../../game/gameobject/CCamera.h"

ComPtr <ID3D11InputLayout> CBillboard::smIL = nullptr;
ComPtr <ID3D11VertexShader> CBillboard::smVS = nullptr;
ComPtr <ID3D11PixelShader> CBillboard::smPS = nullptr;
std::unordered_map<std::string, CBillboard::TextureInfo> CBillboard::smTextureInfo;

CBillboard::CBillboard() :
	mScale({ 1.f,1.f }), mColor({ 1.f,1.f,1.f,1.f }),
	mUV0({ 0.f,0.f }), mUV3({ 1.f,1.f })
{
	static bool ShouldInit = false;

	if (!ShouldInit)
	{
		CBillboard::CreateShader();
		ShouldInit = true;
	}

	Initialize();
}

HRESULT CBillboard::Initialize(void)
{
	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();

	XMFLOAT3 v[4] = {
	{-0.5f ,0.5f, 0.0f },
	{0.5f, 0.5f, 0.0f },
	{-0.5f, -0.5f, 0.0f},
	{0.5f, -0.5f, 0.0f}
	};

	// ���_�o�b�t�@�𐶐�
	bool sts = CreateVertexBuffer(
		device,						// �f�o�C�X�I�u�W�F�N�g
		sizeof(XMFLOAT3),			// �P���_������o�C�g��
		4,							// ���_��
		&v,							// ���_�f�[�^�i�[�������擪�A�h���X
		&mpVetexBuffer				// ���_�o�b�t�@
	);

	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return E_FAIL;
	}

	// �萔�o�b�t�@�𐶐�
	sts = CreateConstantBufferWrite(device, sizeof(CB_DATA), &mpConstantBuffer);
	if (!sts)
	{
		MessageBox(nullptr, "Create Constant Buffer Error", "error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CBillboard::LoadTexture(const std::string pTexFileName)
{
	if (smTextureInfo.count(pTexFileName) == 0)
	{
		smTextureInfo[pTexFileName];

		bool sts = CreateSRVfromFile(pTexFileName.c_str(),
									 CDirectXGraphics::GetInstance()->GetDXDevice(),
									 CDirectXGraphics::GetInstance()->GetImmediateContext(),
									 &smTextureInfo.at(pTexFileName).texRes,
									 &smTextureInfo.at(pTexFileName).texSrv);
		if (!sts)
		{
			MessageBox(nullptr, "Texture Load Error", "error", MB_OK);
			return E_FAIL;
		}
	}

	mpTexRes = smTextureInfo.at(pTexFileName).texRes;
	mpTexSRV = smTextureInfo.at(pTexFileName).texSrv;

	return S_OK;
}

void CBillboard::Update(void)
{
	XMFLOAT4X4 matrixRot;

	XMStoreFloat4x4(&mMatrixWorld, XMMatrixIdentity());
	matrixRot = CCamera::GetInstance()->GetCameraMatrix();
	XMStoreFloat4x4(&matrixRot, XMMatrixInverse(nullptr, XMLoadFloat4x4(&matrixRot)));

	mMatrixWorld._11 = mScale.x * matrixRot._11;
	mMatrixWorld._12 = mScale.x * matrixRot._12;
	mMatrixWorld._13 = mScale.x * matrixRot._13;

	mMatrixWorld._21 = mScale.y * matrixRot._21;
	mMatrixWorld._22 = mScale.y * matrixRot._22;
	mMatrixWorld._23 = mScale.y * matrixRot._23;

	mMatrixWorld._31 = matrixRot._31;
	mMatrixWorld._32 = matrixRot._32;
	mMatrixWorld._33 = matrixRot._33;

	mMatrixWorld._41 = mPos.x;
	mMatrixWorld._42 = mPos.y;
	mMatrixWorld._43 = mPos.z;
}

void CBillboard::Render(void)
{
	ID3D11DeviceContext*		 devicecontext;				// �f�o�C�X�R���e�L�X�g

	// �f�o�C�X�R���e�L�X�g���擾����
	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	unsigned int stride = sizeof(XMFLOAT3);					// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
	unsigned  offset = 0;									// �I�t�Z�b�g�l���Z�b�g

	// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devicecontext->IASetVertexBuffers(
		0,													// �X�^�[�g�X���b�g
		1,													// ���_�o�b�t�@��
		mpVetexBuffer.GetAddressOf(),									// ���_�o�b�t�@�̐擪�A�h���X
		&stride,											// �X�g���C�h
		&offset);											// �I�t�Z�b�g

	// �g�|���W�[���Z�b�g
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�t�H�[�}�b�g���Z�b�g
	devicecontext->IASetInputLayout(smIL.Get());

	devicecontext->VSSetShader(smVS.Get(), nullptr, 0);		// ���_�V�F�[�_�[���Z�b�g
	devicecontext->PSSetShader(smPS.Get(), nullptr, 0);		// �s�N�Z���V�F�[�_�[���Z�b�g

	if (mpTexSRV != nullptr)devicecontext->PSSetShaderResources(0, 1, mpTexSRV.GetAddressOf());  // �V�F�[�_�[���e�N�X�`���ɃA�N�Z�X�ł���悤�ɃZ�b�g

	CB_DATA cbdata;

	cbdata.r = mColor.x;
	cbdata.g = mColor.y;
	cbdata.b = mColor.z;
	cbdata.a = mColor.w;

	cbdata.u0 = mUV0.x;
	cbdata.v0 = mUV0.y;
	cbdata.u3 = mUV3.x;
	cbdata.v3 = mUV3.y;

	D3D11_MAPPED_SUBRESOURCE mapRes;
	HRESULT hr = devicecontext->Map(mpConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);

	if (SUCCEEDED(hr))
	{
		memcpy_s(mapRes.pData, mapRes.RowPitch, &cbdata, sizeof(cbdata));
		devicecontext->Unmap(mpConstantBuffer.Get(), 0);
	}

	devicecontext->VSSetConstantBuffers(3, 1, mpConstantBuffer.GetAddressOf());
	devicecontext->PSSetConstantBuffers(3, 1, mpConstantBuffer.GetAddressOf());

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eWorld, mMatrixWorld);

	// �f�o�C�X�R���e�L�X�g�ɒ��_�f�[�^�𗬂�����
	devicecontext->Draw(
		4,									// ���_��
		0);									// �J�n���_�C���f�b�N�X
}

HRESULT CBillboard::CreateShader(void)
{
	// �f�o�C�X���擾����
	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// ���_�i���C�A�E�g�j�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "SV_POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// �G�������g��
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	bool sts = CreateVertexShader(
		device,								// �f�o�C�X�I�u�W�F�N�g						
		"shader/Billboard.hlsl", // ���_�V�F�[�_�[�\�[�X�t�@�C��					
		"vs_main",								// �G���g���֐�
		"vs_5_0",							// ���_�V�F�[�_�[���f���T�D�O
		layout,								// ���_�f�[�^���C�A�E�g
		numElements,						// ���_�f�[�^�̃G�������g��
		&smVS,					// ���_�V�F�[�_�[�I�u�W�F�N�g
		&smIL);					// ���_�f�[�^���C�A�E�g�I�u�W�F�N�g

	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return E_FAIL;
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = CreatePixelShader(							// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
							device,						// �f�o�C�X�I�u�W�F�N�g
							"shader/ps2d.hlsl",			// �s�N�Z���V�F�[�_�[�\�[�X�t�@�C��
							"ps_main",					// �G���g���֐�
							"ps_5_0",					// �s�N�Z���V�F�[�_�[���f���T�D�O
							&smPS);						// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g

	if (!sts)
	{
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CBillboard::SetRotation(XMFLOAT4X4 _mtx)
{
	DX11MtxMultiply(mMatrixWorld, mMatrixWorld, _mtx);
}

