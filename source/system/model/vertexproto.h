#pragma once
#include	<directxmath.h>

// ���_
struct Vertex {
	DirectX::XMFLOAT3 mPos;		// �ʒu
	DirectX::XMFLOAT3 mNormal;		// �@��
	DirectX::XMFLOAT2 mTex;		// UV���W
};

// �}�e���A��
struct Material {
	float mAlpha;
	DirectX::XMFLOAT3 mDiffuse;
	DirectX::XMFLOAT3 mSpecular;
	float	mSpecularPower;
	DirectX::XMFLOAT3 mAmbient;
	DirectX::XMFLOAT3 mEmissive;

	Material()
		: mAlpha(0),
		mDiffuse(DirectX::XMFLOAT3(0, 0, 0)),
		mSpecular(DirectX::XMFLOAT3(0, 0, 0)),
		mSpecularPower(0),
		mAmbient(DirectX::XMFLOAT3(0, 0, 0)),
		mEmissive(DirectX::XMFLOAT3(0, 0, 0))
	{
	}
};

// �萔�o�b�t�@��`�i�}�e���A���j
struct ConstantBufferMaterial {
	DirectX::XMFLOAT4	AmbientMaterial;		// �����̔��˗�
	DirectX::XMFLOAT4	DiffuseMaterial;		// �f�B�t���[�Y���̔��˗�
	DirectX::XMFLOAT4	SpecularMaterial;		// �X�y�L�������̔��˗�
};