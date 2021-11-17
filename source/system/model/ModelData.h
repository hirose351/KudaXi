#pragma once
#include	<vector>
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<assimp\scene.h>
#include	<map>
#include	<assimp\Importer.hpp>
#include	"Mesh.h"
#include	"../dx11/CDirectxGraphics.h"
#include	"assimpscene.h"
#include	"vertexproto.h"

class ModelData
{
public:
	ModelData();
	~ModelData();

	// �R�s�[�֎~
	ModelData(const ModelData&) = delete;
	ModelData& operator=(const ModelData&) = delete;

	// ���[�u�֎~
	ModelData(ModelData&&) = delete;
	ModelData& operator=(ModelData&&) = delete;

	bool Load(std::string resourcefolder, std::string filename);

	void Draw(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4& mtxworld);

	void Exit();

	const std::vector<Mesh>& GetMeshes()const {
		return mMeshes;
	}

	void LoadMaterial();						// �}�e���A����ǂݍ���
private:
	AssimpScene mAssimpscene;					// assimp scene
	std::vector<Mesh> mMeshes;					// ���b�V���̏W�������f��
	std::string mDirectory;					// �e�N�X�`���t�@�C���̈ʒu
	std::vector<MeshTextureInfo> mTexturesloaded;		// ���Ƀ��[�h����Ă���e�N�X�`��
	std::vector<Material> mMaterials;			// �}�e���A���Q

	void processNode(aiNode* _node, const aiScene* _scene);					// �m�[�h�����
	Mesh processMesh(aiMesh* _mesh, const aiScene* _scene, int _meshidx);	// ���b�V�������
																																				// �}�e���A���ɑΉ������e�N�X�`�����擾����
	std::vector<MeshTextureInfo> LoadMaterialTextures(
		aiMaterial *_mat,
		aiTextureType _type,
		std::string _typeName,
		const aiScene *_scene);
};
