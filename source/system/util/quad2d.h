#pragma once

#include	<d3d11.h>
#include	<wrl/client.h>
#include	<string>
#include	"../dx11/dx11mathutil.h"
#include	"shaderhashmap.h"
#include	"texture_manager.h"
#include	"vector.h"

// comptr
using Microsoft::WRL::ComPtr;

// ��`�N���X
class Quad2D {
private:
	// ���_�f�[�^
	struct Vertex {
		DirectX::XMFLOAT3 pos;		// ���W		
		DirectX::XMFLOAT4 color;	// �J���[
		DirectX::XMFLOAT2 tex;		// �e�N�X�`��
	};

	//DirectX::XMFLOAT4X4		mWorldmtx;				// ���[���h�ϊ��s��
	ComPtr<ID3D11Buffer>	mVertexbuffer;			// ���_�o�b�t�@
	ComPtr<ID3D11Buffer>	mIndexbuffer;			// �C���f�b�N�X�o�b�t�@
	Quad2D::Vertex			mVertex[4];				// ��`�S���_
	DirectX::XMFLOAT4		mColor;					// ���_�J���[
	float					mWidth;					// ��
	float					mHeight;				// ����
	float					mDepth;					// �[�x�l

	TextureInfo mTexInfo;

	DirectX::XMFLOAT2 mUv[4];						// �A�j���[�V�����pUV�e�[�u��
	unsigned int mDivu;								// �摜�̉�������
	unsigned int mDivv;								// �摜�̏c������

public:
	// ��`�̏������@(width�F���`��T�C�Y�@height�F�c�`��T�C�Y�@tex_name�F�摜�̏ꏊ�Ɩ��O�@color�F���_�J���[�@u�F���̕������@v�F�c�̕������@z�F���`��T�C�Y)
	bool Init(int width, int height, const char *tex_name, const DirectX::XMFLOAT4 &color, int _u = 1, int _v = 1, float z = 0.0f);

	// �`��
	void Draw(DirectX::XMFLOAT4X4 _mtx);

	// �e�N�X�`���Ȃ��ŕ`��
	void DrawNoTex();

	// �g��A�k��
	//void SetScale(Float3 _size);

	// �ʒu���Z�b�g
	//void SetPosition(Float3 _pos);

	// ���_�J���[�Z�b�g
	void SetColor(const XMFLOAT4 &_color);

	// Z����]
	//void SetZRotation(float angle);

	// X����]
	//void SetXRotation(float angle);

	// ���[���h�ϊ��s����Z�b�g����
	//void SetMtx(const DirectX::XMFLOAT4X4 &mtx) {
	//	mWorldmtx = mtx;

	// ���_�f�[�^�X�V
	void UpdateVertex(uint32_t width, uint32_t height, const DirectX::XMFLOAT4 &color, float z = 0.0f);

	// ���_�o�b�t�@�X�V
	void UpdateVbuffer();

	// UV���W�ݒ�@(_u�F�����牽�Ԗڂ�  _v�F�ォ�牽�Ԗڂ��@direction�F�`�����(�O���� �P���E)�@�f�t�H���g�͉E)
	void SetTextureUV(int _u, int _v, int direction = 1);
};