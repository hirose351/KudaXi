#pragma once
#include	"component_base.h"
#include	"../../system/model/ModelData.h"

namespace Component
{
	class Model : public ComponentBase
	{
	private:
		enum FILETYPE
		{
			eDATFILE,
			eASSIMPFILE
		};

		FILETYPE				mFileType = eASSIMPFILE;		// �t�@�C���^�C�v
		ModelData				mAssimpFile;					// assimpfile
		ID3D11VertexShader*     mpVertexShader = nullptr;		// ���_�V�F�[�_�[���ꕨ
		ID3D11PixelShader*      mpPixelShader = nullptr;		// �s�N�Z���V�F�[�_�[���ꕨ
		ID3D11InputLayout*      mpVertexLayout = nullptr;		// ���_�t�H�[�}�b�g��`

		ID3D11ShaderResourceView*	mpTexSRV = nullptr;			// �e�N�X�`���r�q�u

	public:
		bool Init(const char* _fileName, const char* _vsFile, const char* _psFile, std::string _texFolder);
		void Uninit();
		void Update(unsigned int _animno, const DirectX::XMFLOAT4X4& _mtxworld);
		void Draw(DirectX::XMFLOAT4X4& _mtxworld);
		// �s�N�Z���V�F�[�_�[�Z�b�g
		bool SetPsShader(const char* _psfile);

		const ModelData& GetModelData() {
			return mAssimpFile;
		}
	};
}
