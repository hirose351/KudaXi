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
	std::vector<MeshTextureInfo> textures;		// ���̃}�e���A���Ɋ֘A�Â���DIFFUSE�e�N�X�`���̃��X�g
	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;

	dev = CDirectXGraphics::GetInstance()->GetDXDevice();
	devcon = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �}�e���A������e�N�X�`�������擾��(��{��1��)���[�v����
	for (unsigned int i = 0; i < _mtrl->GetTextureCount(_type); i++)
	{
		aiString str;

		// �}�e���A�����炉�Ԗڂ̃e�N�X�`���t�@�C�������擾����
		_mtrl->GetTexture(_type, i, &str);

		// �������Ƀ��[�h���ꂽ�e�N�X�`���ł���΃��[�h���X�L�b�v����
		bool skip = false;

		// ���[�h�ς݃e�N�X�`���������[�v����
		for (unsigned int j = 0; j < mTexturesloaded.size(); j++)
		{
			// �t�@�C�����������ł�������ǂݍ��܂Ȃ�
			if (std::strcmp(mTexturesloaded[j].path.c_str(), str.C_Str()) == 0)
			{
				// �ǂݍ��ݍς݂̃e�N�X�`������DIFFUSE�e�N�X�`���̃��X�g�ɃZ�b�g
				textures.push_back(mTexturesloaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // �܂��ǂݍ��܂�Ă��Ȃ������ꍇ
			MeshTextureInfo tex;

			std::string filename = std::string(str.C_Str());
			std::string filenameonly = ExtractFileName(filename, '\\');		// �t�@�C�������擾
			filename = mDirectory + filenameonly;				// ���\�[�X�f�B���N�g���{�t�@�C����
//			MessageBox(nullptr, filename.c_str(), "load tex", MB_OK);

			bool sts = CreateSRVfromFile(
				filename.c_str(),
				dev, devcon, &tex.texres, &tex.texture);
			if (!sts)
			{
				// �������������t�s�e�W�Őݒ肳��Ă����ꍇ�ɑΉ�
				filenameonly = utf8_to_multi_winapi(std::string(filenameonly.c_str()));
				filename = mDirectory + filenameonly;	// ���\�[�X�f�B���N�g���{�t�@�C����

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
			// �e�N�X�`������DIFFUSE�e�N�X�`���̃��X�g�ɃZ�b�g
			textures.push_back(tex);
			this->mTexturesloaded.push_back(tex);	// ���̃��f���Ɋ֘A�Â����e�N�X�`�����X�g�ɃZ�b�g
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

	// �}�e���A�������݂��邩�H
	if (mAssimpscene.GetScene()->HasMaterials())
	{
		// �}�e���A���� �擾
		int nummaterial = mAssimpscene.GetScene()->mNumMaterials;

		for (int i = 0; i < nummaterial; i++)
		{
			Material mtrl;

			// i�Ԗڂ̃}�e���A�����擾
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

	mDirectory = resourcefolder;		// ���̃��f���̃e�N�X�`�������݂���f�B���N�g��

	LoadMaterial();						// ���̃��f���Ŏg�p����Ă���}�e���A�����擾����

	// ai�m�[�h����͂���
	processNode(mAssimpscene.GetScene()->mRootNode, mAssimpscene.GetScene());

	return true;
}

void ModelData::Draw(ID3D11DeviceContext * devcon, XMFLOAT4X4& mtxworld)
{
	for (int i = 0; i < mMeshes.size(); i++)
	{
		// ���[���h�ϊ��s��
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtxworld);
		// �萔�o�b�t�@�Z�b�g����
		mMeshes[i].Draw(devcon);
	}
}

// ���b�V���̉��
Mesh ModelData::processMesh(aiMesh * mesh, const aiScene * scene, int meshidx)
{
	std::vector<Vertex> vertices;			// ���_
	std::vector<unsigned int> indices;		// �ʂ̍\�����
	std::vector<MeshTextureInfo> textures;			// �e�N�X�`��
	Material mtrl;

	// ���_�����擾
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.mPos.x = mesh->mVertices[i].x;
		vertex.mPos.y = mesh->mVertices[i].y;
		vertex.mPos.z = mesh->mVertices[i].z;

		// �@���x�N�g�������݂��邩�H
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

		// �e�N�X�`�����W�i�O�Ԗځj�����݂��邩�H
		if (mesh->HasTextureCoords(0))
		{
			vertex.mTex.x = mesh->mTextureCoords[0][i].x;
			vertex.mTex.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	// �e�N�X�`�������擾����
	if (mesh->mMaterialIndex >= 0)
	{
		// ���̃��b�V���̃}�e���A���C���f�b�N�X�l���擾����
		int	mtrlidx = mesh->mMaterialIndex;

		// ���̃��b�V���̃}�e���A�����擾����
		mtrl = mMaterials[mtrlidx];

		// �V�[������}�e���A���f�[�^���擾����
		aiMaterial* material = scene->mMaterials[mtrlidx];

		// ���̃}�e���A���Ɋ֘A�Â����e�N�X�`�������o��
		std::vector<MeshTextureInfo> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);

		// ���̃��b�V���Ŏg�p���Ă���e�N�X�`����ۑ�
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}

	// �ʂ̍\�������擾
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
	// �e�N�X�`�����\�[�X���������
	for (auto tex : mTexturesloaded)
	{
		if (tex.texture != nullptr)
		{
			tex.texture->Release();
		}
	}

	// ���b�V���̉��
	for (int i = 0; i < mMeshes.size(); i++)
	{
		mMeshes[i].Close();
	}

	// assimp scene ���
	mAssimpscene.Exit();

}

// �m�[�h�̉��
void ModelData::processNode(aiNode * node, const aiScene * scene)
{
	// �m�[�h���̃��b�V���̐������[�v����
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		int meshindex = node->mMeshes[i];			// �m�[�h��i�Ԗڃ��b�V���̃C���f�b�N�X���擾
		aiMesh* mesh = scene->mMeshes[meshindex];	// �V�[�����烁�b�V���{�̂����o��

		mMeshes.push_back(this->processMesh(mesh, scene, meshindex));
	}

	// �q�m�[�h�ɂ��Ă����
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}