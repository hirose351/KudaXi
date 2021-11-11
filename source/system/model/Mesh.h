#pragma once
#include	<vector>
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<string>
#include	<wrl/client.h>
#include	"vertexproto.h"

// �e�N�X�`���\����
struct Texture {
	std::string type;							// �e�N�X�`�����
	std::string path;							// �e�N�X�`���p�X��
	ID3D11Resource*	texres;						// �e�N�X�`�����\�[�X
	ID3D11ShaderResourceView *texture;			// SRV
};

using Microsoft::WRL::ComPtr;

class Mesh {
public:
	ID3D11ShaderResourceView*    mWhitesrv;	// �^�����r�q�u
	ID3D11Resource*				mWhiteres;	// �^����RES

	std::vector<Vertex> mVertices;			// ���_�f�[�^
	std::vector<unsigned int> mIndices;	// �C���f�b�N�X
	std::vector<Texture> mTextures;		// �e�N�X�`��
	ID3D11Device* mDev;					// �f�o�C�X	
	Material mMtrl;						// �}�e���A��

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material mtrl);

	void Draw(ID3D11DeviceContext *devcon);
	void Close();
private:
	ID3D11Buffer*  mVertexBuffer = nullptr;	// ���_�o�b�t�@
	ID3D11Buffer*  mIndexBuffer = nullptr;		// �C���f�b�N�X�o�b�t�@
	ID3D11Buffer*  mCbmtrl = nullptr;			// �}�e���A���p�R���X�^���g�o�b�t�@

	bool SetupMesh();
};