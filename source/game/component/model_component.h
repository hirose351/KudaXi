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

		FILETYPE				mFileType = eASSIMPFILE;		// ファイルタイプ
		ModelData				mAssimpFile;					// assimpfile
		ID3D11VertexShader*     mpVertexShader = nullptr;		// 頂点シェーダー入れ物
		ID3D11PixelShader*      mpPixelShader = nullptr;		// ピクセルシェーダー入れ物
		ID3D11InputLayout*      mpVertexLayout = nullptr;		// 頂点フォーマット定義

		ID3D11ShaderResourceView*	mpTexSRV = nullptr;			// テクスチャＳＲＶ

	public:
		bool Init(const char* _fileName, const char* _vsFile, const char* _psFile, std::string _texFolder);
		void Uninit();
		void Update(unsigned int _animno, const DirectX::XMFLOAT4X4& _mtxworld);
		void Draw(DirectX::XMFLOAT4X4& _mtxworld);
		// ピクセルシェーダーセット
		bool SetPsShader(const char* _psfile);

		const ModelData& GetModelData() {
			return mAssimpFile;
		}
	};
}
