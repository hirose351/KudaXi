#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<math.h>
#include	<vector>
#include	<wrl/client.h>
#include	"../dx11/Shader.h"

constexpr float PI = DirectX::XM_PI;

using Microsoft::WRL::ComPtr;

class Sphere {
private:
	ComPtr<ID3D11Buffer> mpVertexBuffer;		// ���_�o�b�t�@
	ComPtr<ID3D11Buffer> mpIndexBuffer;		// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D11VertexShader> mpVertexShader;	// ���_�V�F�[�_�[
	ComPtr<ID3D11PixelShader> mpPixelShader;	// �s�N�Z���V�F�[�_�[
	ComPtr<ID3D11InputLayout>  mpVertexLayout;	// ���_���C�A�E�g
	unsigned int		mDivX;					// ���������̕�����
	unsigned int		mDivY;					// ���������̕�����
	float				mRadius;				// ���a
	XMFLOAT4			mColor;				// ���_�J���[

	// ���_�f�[�^
	struct Vertex {
		DirectX::XMFLOAT3	Pos;
		DirectX::XMFLOAT3	Normal;
		DirectX::XMFLOAT4	Color;
	};

	// �C���f�b�N�X�f�[�^
	struct Face {
		unsigned int idx[3];
	};

	std::vector<Vertex>		mVertex;			//�@���_�f�[�^
	std::vector<Face>		mFace;				//	�C���f�b�N�X�f�[�^

	// �C���f�b�N�X�f�[�^���쐬����
	void CreateIndex();
	// ���_�f�[�^���쐬����
	void CreateVertex();
	// ���K��
	void Normalize(XMFLOAT3 _vector, XMFLOAT3& _Normal);
public:
	// �`��
	void Draw(ID3D11DeviceContext* _device);

	// ������
	bool Init(float r,						// ���a
			  int _divisionHorizontal,			// ���������̕�����
			  int _divisionVertical,				// ���������̕�����
			  XMFLOAT4	_color,					// ���_�J���[
			  ID3D11Device* _device);
};