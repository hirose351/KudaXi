#pragma once
#include	<DirectXMath.h>
#include	"Shader.h"
#include	"../../Memory.h"
#include	"CDirectxGraphics.h"

class DX11SetTransform {
private:
	DX11SetTransform() {}
public:
	DX11SetTransform(const DX11SetTransform&) = delete;
	DX11SetTransform& operator=(const DX11SetTransform&) = delete;
	DX11SetTransform(DX11SetTransform&&) = delete;
	DX11SetTransform& operator=(DX11SetTransform&&) = delete;

	static DX11SetTransform* GetInstance() {
		static DX11SetTransform instance;
		return &instance;
	}
	// �萔�o�b�t�@��`(���[���h�ϊ��s��)
	ALIGN16 struct ConstantBufferWorld
	{
		XMMATRIX World;				// ���[���h�ϊ��s��
	};
	// �萔�o�b�t�@��`(�r���[�ϊ��s��)
	ALIGN16 struct ConstantBufferView
	{
		XMMATRIX View;				// �r���[�ϊ��s��
	};
	// �萔�o�b�t�@��`(���t���[���ݒ�)
	ALIGN16 struct ConstantBufferProjection
	{
		XMMATRIX Projection;		// �v���W�F�N�V�����ϊ��s��
	};

	enum class TYPE {
		eWorld,
		eView,
		eProjection
	};

	void Uninit() {
		if (mpConstantBufferWorld)
		{
			mpConstantBufferWorld->Release();
			mpConstantBufferWorld = nullptr;
		}
		if (mpConstantBufferView)
		{
			mpConstantBufferView->Release();
			mpConstantBufferView = nullptr;
		}
		if (mpConstantBufferProjection)
		{
			mpConstantBufferProjection->Release();
			mpConstantBufferProjection = nullptr;
		}
	}

	bool Init() {

		ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();

		// �R���X�^���g�o�b�t�@�쐬
		bool sts = CreateConstantBuffer(
			device,			// �f�o�C�X
			sizeof(ConstantBufferWorld),	// �T�C�Y
			&mpConstantBufferWorld);			// �R���X�^���g�o�b�t�@�Q
		if (!sts)
		{
			MessageBox(NULL, "CreateBuffer(constant buffer world) error", "Error", MB_OK);
			return false;
		}

		// �R���X�^���g�o�b�t�@�쐬
		sts = CreateConstantBuffer(
			device,			// �f�o�C�X
			sizeof(ConstantBufferView),	// �T�C�Y
			&mpConstantBufferView);			// �R���X�^���g�o�b�t�@�Q
		if (!sts)
		{
			MessageBox(NULL, "CreateBuffer(constant buffer view) error", "Error", MB_OK);
			return false;
		}

		// �R���X�^���g�o�b�t�@�쐬
		sts = CreateConstantBuffer(
			device,			// �f�o�C�X
			sizeof(ConstantBufferProjection),	// �T�C�Y
			&mpConstantBufferProjection);			// �R���X�^���g�o�b�t�@�Q
		if (!sts)
		{
			MessageBox(NULL, "CreateBuffer(constant buffer projection) error", "Error", MB_OK);
			return false;
		}
		return true;
	}

	void SetTransform(TYPE type, DirectX::XMFLOAT4X4& matrix) {
		ALIGN16 XMMATRIX mat;
		mat = DirectX::XMLoadFloat4x4(&matrix);

		ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

		switch (type)
		{
		case TYPE::eWorld:
			mCbWorld.World = XMMatrixTranspose(mat);
			devicecontext->UpdateSubresource(mpConstantBufferWorld, 0, nullptr, &mCbWorld, 0, 0);
			devicecontext->VSSetConstantBuffers(0, 1, &mpConstantBufferWorld);
			devicecontext->PSSetConstantBuffers(0, 1, &mpConstantBufferWorld);
			break;
		case TYPE::eView:
			mCbView.View = XMMatrixTranspose(mat);
			devicecontext->UpdateSubresource(mpConstantBufferView, 0, nullptr, &mCbView, 0, 0);
			devicecontext->VSSetConstantBuffers(1, 1, &mpConstantBufferView);
			devicecontext->PSSetConstantBuffers(1, 1, &mpConstantBufferView);
			break;
		case TYPE::eProjection:
			mCbProjection.Projection = XMMatrixTranspose(mat);
			devicecontext->UpdateSubresource(mpConstantBufferProjection, 0, nullptr, &mCbProjection, 0, 0);
			devicecontext->VSSetConstantBuffers(2, 1, &mpConstantBufferProjection);
			devicecontext->PSSetConstantBuffers(2, 1, &mpConstantBufferProjection);
			break;
		}
	}
private:
	ID3D11Buffer * mpConstantBufferWorld = nullptr;		// �R���X�^���g�o�b�t�@2
	ID3D11Buffer * mpConstantBufferView = nullptr;			// �R���X�^���g�o�b�t�@2
	ID3D11Buffer * mpConstantBufferProjection = nullptr;	// �R���X�^���g�o�b�t�@2
	ConstantBufferWorld mCbWorld;				// b0 
	ConstantBufferView mCbView;				// b1
	ConstantBufferProjection mCbProjection;	// b2
};