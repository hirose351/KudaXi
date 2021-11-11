#pragma once
#include	<DirectXMath.h>
#include	"memory.h"
#include	"../../system/dx11/Shader.h"
#include	"DX11util.h"

using namespace DirectX;

class CLight {
	ALIGN16 struct ConstantBufferLight {
		XMFLOAT4 LightDirection;
		XMFLOAT4 EyePos;
		XMFLOAT4 Ambient;
	};

	enum class LightType {
		DIRECTIONAL,
		POINT,
		SPOT
	};
	LightType			mType;
	XMFLOAT3			mEyepos;
	XMFLOAT4			mLightpos;		// w=0�̎��͕��� w=1�̎��͈ʒu
	ID3D11Buffer*       mpConstantBufferLight = nullptr;
	XMFLOAT4			mAmbient;
public:
	bool Init(DirectX::XMFLOAT3 _eyepos, DirectX::XMFLOAT4 lightpos) {
		mLightpos = lightpos;
		mEyepos = _eyepos;
		mType = LightType::DIRECTIONAL;

		// �R���X�^���g�o�b�t�@�쐬
		bool sts = CreateConstantBuffer(
			GetDX11Device(),				// �f�o�C�X
			sizeof(ConstantBufferLight),		// �T�C�Y
			&mpConstantBufferLight);			// �R���X�^���g�o�b�t�@�S
		if (!sts)
		{
			MessageBox(NULL, "CreateBuffer(constant buffer Light) error", "Error", MB_OK);
			return false;
		}

		Update();

		return true;
	}

	void Update() {
		ConstantBufferLight		cb;

		cb.EyePos.w = 1.0;
		cb.EyePos.x = mEyepos.x;
		cb.EyePos.y = mEyepos.y;
		cb.EyePos.z = mEyepos.z;

		cb.LightDirection.x = mLightpos.x;
		cb.LightDirection.y = mLightpos.y;
		cb.LightDirection.z = mLightpos.z;
		cb.LightDirection.w = mLightpos.w;

		cb.Ambient = mAmbient;

		GetDX11DeviceContext()->UpdateSubresource(mpConstantBufferLight,
												  0,
												  nullptr,
												  &cb,
												  0, 0);

		// �R���X�^���g�o�b�t�@4����3���W�X�^�փZ�b�g�i���_�V�F�[�_�[�p�j
		GetDX11DeviceContext()->VSSetConstantBuffers(4, 1, &mpConstantBufferLight);
		// �R���X�^���g�o�b�t�@4����3���W�X�^�փZ�b�g(�s�N�Z���V�F�[�_�[�p)
		GetDX11DeviceContext()->PSSetConstantBuffers(4, 1, &mpConstantBufferLight);

	}

	void Uninit() {
		if (mpConstantBufferLight)
		{
			mpConstantBufferLight->Release();
			mpConstantBufferLight = nullptr;
		}
	}

	void SetEyePos(const XMFLOAT3 &eyepos) {
		mEyepos = eyepos;
	}

	void SetLightPos(const XMFLOAT4 &lightpos) {
		mLightpos = lightpos;
	}

	void SetAmbient(const XMFLOAT4 &amb) {
		mAmbient = amb;
	}
};