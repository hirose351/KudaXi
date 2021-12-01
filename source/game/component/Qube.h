#pragma once
#include	<wrl/client.h>
#include	"../../system/dx11/Shader.h"
#include	"../gameobject/primitive.h"
#include	"../gameobject/transform.h"

constexpr float PI = DirectX::XM_PI;

using Microsoft::WRL::ComPtr;

class Qube {
private:
	ComPtr<ID3D11Buffer> mpVertexBuffer;		// ���_�o�b�t�@
	ComPtr<ID3D11Buffer> mpIndexBuffer;		// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D11VertexShader> mpVertexShader;	// ���_�V�F�[�_�[
	ComPtr<ID3D11PixelShader> mpPixelShader;	// �s�N�Z���V�F�[�_�[
	ComPtr<ID3D11InputLayout>  mpVertexLayout;	// ���_���C�A�E�g
	Primitive::AABB mAabb;
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
	// ������
	bool Init(const Primitive::AABB& _aabb, const XMFLOAT4& color);
	// �X�V
	void Update(const Primitive::AABB& _aabb);
	// �`��
	void Draw(DirectX::XMFLOAT4X4 _mtx);
};

