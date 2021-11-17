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

	// コピー禁止
	ModelData(const ModelData&) = delete;
	ModelData& operator=(const ModelData&) = delete;

	// ムーブ禁止
	ModelData(ModelData&&) = delete;
	ModelData& operator=(ModelData&&) = delete;

	bool Load(std::string resourcefolder, std::string filename);

	void Draw(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4& mtxworld);

	void Exit();

	const std::vector<Mesh>& GetMeshes()const {
		return mMeshes;
	}

	void LoadMaterial();						// マテリアルを読み込む
private:
	AssimpScene mAssimpscene;					// assimp scene
	std::vector<Mesh> mMeshes;					// メッシュの集合がモデル
	std::string mDirectory;					// テクスチャファイルの位置
	std::vector<MeshTextureInfo> mTexturesloaded;		// 既にロードされているテクスチャ
	std::vector<Material> mMaterials;			// マテリアル群

	void processNode(aiNode* _node, const aiScene* _scene);					// ノードを解析
	Mesh processMesh(aiMesh* _mesh, const aiScene* _scene, int _meshidx);	// メッシュを解析
																																				// マテリアルに対応したテクスチャを取得する
	std::vector<MeshTextureInfo> LoadMaterialTextures(
		aiMaterial *_mat,
		aiTextureType _type,
		std::string _typeName,
		const aiScene *_scene);
};
