#include	<assimp\Importer.hpp>
#include	<assimp\scene.h>
#include	<assimp\postprocess.h>
#include	<assimp/cimport.h>
#include	<unordered_map>
#include	<vector>
#include	"../dx11/Shader.h"
#include	"../dx11/DX11Settransform.h"
#include	"../dx11/CDirectxGraphics.h"
#include	"memory.h"
#include	"ModelData.h"
#include	"vertexproto.h"
#include	"utftosjisconv.h"

std::vector<MeshTextureInfo> ModelData::LoadMaterialTextures(
	aiMaterial* _mtrl,
	aiTextureType _type,
	std::string _typeName,
	const aiScene * _scene)
{
	std::vector<MeshTextureInfo> textures;		// このマテリアルに関連づいたDIFFUSEテクスチャのリスト
	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;

	dev = CDirectXGraphics::GetInstance()->GetDXDevice();
	devcon = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// マテリアルからテクスチャ個数を取得し(基本は1個)ループする
	for (unsigned int i = 0; i < _mtrl->GetTextureCount(_type); i++)
	{
		aiString str;

		// マテリアルからｉ番目のテクスチャファイル名を取得する
		_mtrl->GetTexture(_type, i, &str);

		// もし既にロードされたテクスチャであればロードをスキップする
		bool skip = false;

		// ロード済みテクスチャ数分ループする
		for (unsigned int j = 0; j < mTexturesloaded.size(); j++)
		{
			// ファイル名が同じであったら読み込まない
			if (std::strcmp(mTexturesloaded[j].path.c_str(), str.C_Str()) == 0)
			{
				// 読み込み済みのテクスチャ情報をDIFFUSEテクスチャのリストにセット
				textures.push_back(mTexturesloaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // まだ読み込まれていなかった場合
			MeshTextureInfo tex;

			std::string filename = std::string(str.C_Str());
			std::string filenameonly = ExtractFileName(filename, '\\');		// ファイル名を取得
			filename = mDirectory + filenameonly;				// リソースディレクトリ＋ファイル名
//			MessageBox(nullptr, filename.c_str(), "load tex", MB_OK);

			bool sts = CreateSRVfromFile(
				filename.c_str(),
				dev, devcon, &tex.texres, &tex.texture);
			if (!sts)
			{
				// ｆｉｌｅ名がＵＴＦ８で設定されていた場合に対応
				filenameonly = utf8_to_multi_winapi(std::string(filenameonly.c_str()));
				filename = mDirectory + filenameonly;	// リソースディレクトリ＋ファイル名

				bool sts = CreateSRVfromFile(
					filename.c_str(),
					dev, devcon, &tex.texres, &tex.texture);
				if (!sts)
				{
					MessageBox(nullptr, "Texture couldn't be loaded", "Error!", MB_ICONERROR | MB_OK);
					tex.texture = nullptr;
				}
			}

			tex.type = _typeName;
			tex.path = str.C_Str();
			// テクスチャ情報をDIFFUSEテクスチャのリストにセット
			textures.push_back(tex);
			this->mTexturesloaded.push_back(tex);	// このモデルに関連づいたテクスチャリストにセット
		}
	}

	return textures;
}

ModelData::ModelData()
{
}

ModelData::~ModelData()
{
	Exit();
}

void ModelData::LoadMaterial() {

	// マテリアルが存在するか？
	if (mAssimpscene.GetScene()->HasMaterials())
	{
		// マテリアル数 取得
		int nummaterial = mAssimpscene.GetScene()->mNumMaterials;

		for (int i = 0; i < nummaterial; i++)
		{
			Material mtrl;

			// i番目のマテリアルを取得
			aiMaterial* mat = mAssimpscene.GetScene()->mMaterials[i];

			aiColor3D colordiffuse(1.f, 1.f, 1.f);
			mat->Get(AI_MATKEY_COLOR_DIFFUSE, colordiffuse);
			mtrl.mDiffuse.x = colordiffuse.r;
			mtrl.mDiffuse.y = colordiffuse.g;
			mtrl.mDiffuse.z = colordiffuse.b;
			mtrl.mDiffuse.x = 1.0f;
			mtrl.mDiffuse.y = 1.0f;
			mtrl.mDiffuse.z = 1.0f;

			aiColor3D colorspecular(0.f, 0.f, 0.f);
			mat->Get(AI_MATKEY_COLOR_SPECULAR, colorspecular);
			mtrl.mSpecular.x = colorspecular.r;
			mtrl.mSpecular.y = colorspecular.g;
			mtrl.mSpecular.z = colorspecular.b;

			aiColor3D colorambient(0.f, 0.f, 0.f);
			mat->Get(AI_MATKEY_COLOR_AMBIENT, colorambient);
			mtrl.mAmbient.x = colorambient.r;
			mtrl.mAmbient.y = colorambient.g;
			mtrl.mAmbient.z = colorambient.b;

			aiColor3D coloremissive(0.f, 0.f, 0.f);
			mat->Get(AI_MATKEY_COLOR_EMISSIVE, coloremissive);
			mtrl.mEmissive.x = coloremissive.r;
			mtrl.mEmissive.y = coloremissive.g;
			mtrl.mEmissive.z = coloremissive.b;

			float power;
			mat->Get(AI_MATKEY_SHININESS_STRENGTH, power);
			mtrl.mSpecularPower = power;

			mMaterials.push_back(mtrl);
		}
	}
}

bool ModelData::Load(std::string resourcefolder,
					 std::string filename)
{
	bool sts = mAssimpscene.Init(filename);
	if (!sts)
	{
		MessageBox(nullptr, "ModelData load error", "error", MB_OK);
		return false;
	}

	mDirectory = resourcefolder;		// このモデルのテクスチャが存在するディレクトリ

	LoadMaterial();						// このモデルで使用されているマテリアルを取得する

	// aiノードを解析する
	processNode(mAssimpscene.GetScene()->mRootNode, mAssimpscene.GetScene());

	return true;
}

void ModelData::Draw(ID3D11DeviceContext * devcon, XMFLOAT4X4& mtxworld)
{
	for (int i = 0; i < mMeshes.size(); i++)
	{
		// ワールド変換行列
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtxworld);
		// 定数バッファセット処理
		mMeshes[i].Draw(devcon);
	}
}

// メッシュの解析
Mesh ModelData::processMesh(aiMesh * mesh, const aiScene * scene, int meshidx)
{
	std::vector<Vertex> vertices;			// 頂点
	std::vector<unsigned int> indices;		// 面の構成情報
	std::vector<MeshTextureInfo> textures;			// テクスチャ
	Material mtrl;

	// 頂点情報を取得
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.mPos.x = mesh->mVertices[i].x;
		vertex.mPos.y = mesh->mVertices[i].y;
		vertex.mPos.z = mesh->mVertices[i].z;

		// 法線ベクトルが存在するか？
		if (mesh->HasNormals())
		{
			vertex.mNormal.x = mesh->mNormals[i].x;
			vertex.mNormal.y = mesh->mNormals[i].y;
			vertex.mNormal.z = mesh->mNormals[i].z;
		}
		else
		{
			vertex.mNormal.x = 0.0f;
			vertex.mNormal.y = 0.0f;
			vertex.mNormal.z = 0.0f;
		}

		// テクスチャ座標（０番目）が存在するか？
		if (mesh->HasTextureCoords(0))
		{
			vertex.mTex.x = mesh->mTextureCoords[0][i].x;
			vertex.mTex.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	// テクスチャ情報を取得する
	if (mesh->mMaterialIndex >= 0)
	{
		// このメッシュのマテリアルインデックス値を取得する
		int	mtrlidx = mesh->mMaterialIndex;

		// このメッシュのマテリアルを取得する
		mtrl = mMaterials[mtrlidx];

		// シーンからマテリアルデータを取得する
		aiMaterial* material = scene->mMaterials[mtrlidx];

		// このマテリアルに関連づいたテクスチャを取り出す
		std::vector<MeshTextureInfo> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);

		// このメッシュで使用しているテクスチャを保存
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}

	// 面の構成情報を取得
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return Mesh(vertices, indices, textures, mtrl);
}

void ModelData::Exit()
{
	// テクスチャリソースを解放する
	for (auto tex : mTexturesloaded)
	{
		if (tex.texture != nullptr)
		{
			tex.texture->Release();
		}
	}

	// メッシュの解放
	for (int i = 0; i < mMeshes.size(); i++)
	{
		mMeshes[i].Close();
	}

	// assimp scene 解放
	mAssimpscene.Exit();

}

// ノードの解析
void ModelData::processNode(aiNode * node, const aiScene * scene)
{
	// ノード内のメッシュの数分ループする
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		int meshindex = node->mMeshes[i];			// ノードのi番目メッシュのインデックスを取得
		aiMesh* mesh = scene->mMeshes[meshindex];	// シーンからメッシュ本体を取り出す

		mMeshes.push_back(this->processMesh(mesh, scene, meshindex));
	}

	// 子ノードについても解析
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}