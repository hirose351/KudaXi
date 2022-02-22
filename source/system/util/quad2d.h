#pragma once
#include	<d3d11.h>
#include	<wrl/client.h>
#include	<string>
#include	"../dx11/dx11_mathutil.h"
#include	"shaderhashmap.h"
#include	"texture_manager.h"
#include	"vector.h"

// comptr
using Microsoft::WRL::ComPtr;

// �`�悷��^�C�v
enum class DrawType {
	eTex,
	eNoTex,
	eMokomokoShaderTex
};

// ��`�N���X
class CQuad2D {
private:
	// ���_�f�[�^
	struct Vertex {
		DirectX::XMFLOAT3 pos;		// ���W		
		DirectX::XMFLOAT4 color;	// �J���[
		DirectX::XMFLOAT2 tex;		// �e�N�X�`��
	};

	ComPtr<ID3D11Buffer>	mVertexbuffer;			// ���_�o�b�t�@
	ComPtr<ID3D11Buffer>	mIndexbuffer;			// �C���f�b�N�X�o�b�t�@
	CQuad2D::Vertex			mVertex[4];				// ��`�S���_
	DirectX::XMFLOAT4		mColor;					// ���_�J���[
	DirectX::XMFLOAT2		mScale;					// �傫��
	float					mDepth;					// �[�x�l

	TextureInfo mTexInfo;

	DirectX::XMFLOAT2 mUv[4];						// �A�j���[�V�����pUV�e�[�u��
	unsigned int mDivu;								// �摜�̉�������
	unsigned int mDivv;								// �摜�̏c������

	ALIGN16 struct ConstantBufferTime {
		float time;
	};

	ID3D11Buffer*       mpConstantBufferTime = nullptr;

public:
	// ��`�̏������@(_scale�F�傫���@_texName�F�摜�̏ꏊ�Ɩ��O�@_color�F���_�J���[�@_u�F���̕������@_v�F�c�̕������@_z�F���`��T�C�Y)
	bool Init(DirectX::XMFLOAT2 _scale, std::string _texName, const DirectX::XMFLOAT4 &_color, int _u = 1, int _v = 1, float _z = 0.0f);

	// �`��
	void Draw(DirectX::XMFLOAT4X4 _mtx);

	// �e�N�X�`���Ȃ��ŕ`��
	void DrawNoTex(DirectX::XMFLOAT4X4 _mtx);

	// ����V�F�[�_�[�ŕ`��
	void DrawMokoMokoShaderTex(DirectX::XMFLOAT4X4 _mtx);

	// ���_�J���[�Z�b�g
	void SetColor(const XMFLOAT4 &_color);

	// ���_�f�[�^�X�V
	void UpdateVertex(DirectX::XMFLOAT2 _scale, const DirectX::XMFLOAT4 &color, float z = 0.0f);

	// ���_�o�b�t�@�X�V
	void UpdateVbuffer();

	// UV���W�ݒ�@(_u�F�����牽�Ԗڂ�  _v�F�ォ�牽�Ԗڂ��@direction�F�`�����(�O���� �P���E)�@�f�t�H���g�͉E)
	void SetTextureUV(int _u, int _v, int direction = 1);
};