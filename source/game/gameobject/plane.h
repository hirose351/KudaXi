#pragma once
#include	"gameobject.h"
#include	"transform.h"
#include	"../../system/model/vertexproto.h"
#include	"../../system/util/texture_manager.h"

class Plane : public GameObject, Transform
{
private:
	TextureInfo* mTexInfo[4];

	ComPtr<ID3D11Buffer> mpVertexBuffer;		// ���_�o�b�t�@
	//ComPtr<ID3D11Buffer> mpIndexBuffer;			// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D11VertexShader> mpVertexShader;	// ���_�V�F�[�_�[
	ComPtr<ID3D11PixelShader>  mpPixelShader;	// �s�N�Z���V�F�[�_�[
	ComPtr<ID3D11InputLayout>  mpVertexLayout;	// ���_���C�A�E�g

	int mMapChipSize = 15;

	// ���ʃ`�b�v
	Vertex mVartex[4] = {
		// ���_���W															�@���x�N�g��								�e�N�X�`�����W
		{ DirectX::XMFLOAT3(-mMapChipSize / 2.0f, 0,  mMapChipSize / 2.0f), DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(mMapChipSize / 2.0f, 0,  mMapChipSize / 2.0f), DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-mMapChipSize / 2.0f, 0, -mMapChipSize / 2.0f), DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(mMapChipSize / 2.0f, 0, -mMapChipSize / 2.0f), DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT2(0.0f, 1.0f) }
	};

	// todo:�X�e�[�W�ɂ���ăe�N�X�`��������
	const char*	mTexFileName[4] =
	{
		"assets/image/stage/BLACK.png",
		"assets/image/stage/STAGE.png",
		"assets/image/stage/STAGE.png",
		"assets/image/stage/STAGE.png",
	};

public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void Finalize()override;
};
