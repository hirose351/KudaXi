//*****************************************************************************
//!	@file	CBoundingSphere.h
//!	@brief	���E���N���X
//!	@note	
//!	@author
//*****************************************************************************
#pragma once

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<DirectXMath.h>
#include	"Sphere.h"
#include	"../dx11/dx11mathutil.h"
#include	"../dx11/DX11util.h"
#include	"../dx11/DX11Settransform.h"

//-----------------------------------------------------------------------------
// �}�N���錾
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
struct	HITDATA {
	float	minx, miny, minz;			// �o�E���f�B���O�{�b�N�X�ŏ��l	
	float	maxx, maxy, maxz;			// �o�E���f�B���O�{�b�N�X�ő�l
	float	cx, cy, cz;					// �o�E���f�B���O�X�t�B�A���S���W
	float	r;							// �o�E���f�B���O�X�t�B�A���a
};

class BoundingSphere {
private:
	HITDATA				mHitData;				// �����蔻��p�f�[�^
	DirectX::XMFLOAT3	mWpos;					// ���[���h��ԏ�̈ʒu���W
	DirectX::XMFLOAT4X4	mMatrix;				// ���E���̕`��p�s��
	Sphere				mSphereMesh;			// �����b�V��

	void CalculateBoundingSphere(std::vector<XMFLOAT3>& vertices);
	void CreateSphereMesh(float r, DirectX::XMFLOAT4 color) {
		// �o�E���f�B���O�X�t�B�A��\�������b�V���𐶐�
		bool sts = mSphereMesh.Init(r,				// ���a
									50,								// ���������̕�����
									50,								// ���������̕�����
									color,
									GetDX11Device());				// �f�o�C�X�I�u�W�F�N�g
		if (!sts)
		{
			MessageBox(nullptr, "CSphere init error", "error", MB_OK);
		}
	}
public:
	BoundingSphere() {
	}

	~BoundingSphere() {
		Exit();
	}

	void Init(std::vector<XMFLOAT3>& vertices, XMFLOAT4 color) {
		CalculateBoundingSphere(vertices);			// �q�b�g�f�[�^�̌v�Z
		CreateSphereMesh(mHitData.r, color);		// �����b�V�����쐬����
	}

	void UpdatePosition(const DirectX::XMFLOAT4X4& mat) {
		DirectX::XMFLOAT3 pos(mHitData.cx, mHitData.cy, mHitData.cz);

		// �s��~�x�N�g���i���[���h���W�ɕϊ��j
		DX11Vec3MulMatrix(mWpos, pos, mat);

		mMatrix = mat;
		mMatrix._41 = mWpos.x;
		mMatrix._42 = mWpos.y;
		mMatrix._43 = mWpos.z;
	}

	void Exit() {
	}

	void Draw() {
		ID3D11DeviceContext* device;
		device = GetDX11DeviceContext();
		// ���[���h�ϊ��s��
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eWorld, mMatrix);
		mSphereMesh.Draw(device);
	}

	float GetR() const {
		return mHitData.r;
	}

	const DirectX::XMFLOAT3& GetWPos() {
		return mWpos;
	}

	const HITDATA& GetHitdata() {
		return mHitData;
	}
};

//******************************************************************************
//	End of file.
//******************************************************************************