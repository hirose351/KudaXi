#pragma once
#include "component_base.h"
#include	<d3d11.h>
#include	<vector>
#include	<wrl/client.h>
#include	"../../system/model/ModelData.h"

using Microsoft::WRL::ComPtr;

class ModelComponent : public ComponentBase
{
private:
	enum FILETYPE {
		eDATFILE,
		eASSIMPFILE
	};
	FILETYPE				mFileType = eASSIMPFILE;		// �t�@�C���^�C�v
	ModelData				mAssimpFile;					// assimpfile
	ID3D11VertexShader*     mpVertexShader = nullptr;		// ���_�V�F�[�_�[���ꕨ
	ID3D11PixelShader*      mpPixelShader = nullptr;		// �s�N�Z���V�F�[�_�[���ꕨ
	ID3D11InputLayout*      mpVertexLayout = nullptr;		// ���_�t�H�[�}�b�g��`

	ID3D11ShaderResourceView*	mTexSRV = nullptr;			// �e�N�X�`���r�q�u

public:
	ModelComponent(const char * _fileName, const char * _vsFile, const char * _psFile, std::string _texFolder);

	//void Update(
		//unsigned int _animno,
		//const DirectX::XMFLOAT4X4& _mtxworld);

	void Draw()override;
	void Uninit() override;

	// �s�N�Z���V�F�[�_�[�Z�b�g
	bool SetPsShader(const char* _psfile);

	const ModelData& GetModelData() {
		return mAssimpFile;
	}
};