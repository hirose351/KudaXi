#pragma once
#include	<d3d11.h>
#include	<vector>
#include	<wrl/client.h>
#include	"ModelData.h"

using Microsoft::WRL::ComPtr;

class CModel {
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
	bool Init(const char* _filename, const char* _vsfile, const char* _psfile, std::string _texfolder);
	void Uninit();
	void Update(
		unsigned int _animno,
		const DirectX::XMFLOAT4X4& _mtxworld);
	void Draw(DirectX::XMFLOAT4X4& _mtxworld);
	// �s�N�Z���V�F�[�_�[�Z�b�g
	bool SetPsShader(const char* _psfile);

	const ModelData& GetModelData() {
		return mAssimpFile;
	}
};
