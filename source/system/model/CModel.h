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
	FILETYPE				mFileType = eASSIMPFILE;		// ファイルタイプ
	ModelData				mAssimpFile;					// assimpfile
	ID3D11VertexShader*     mpVertexShader = nullptr;		// 頂点シェーダー入れ物
	ID3D11PixelShader*      mpPixelShader = nullptr;		// ピクセルシェーダー入れ物
	ID3D11InputLayout*      mpVertexLayout = nullptr;		// 頂点フォーマット定義

	ID3D11ShaderResourceView*	mTexSRV = nullptr;			// テクスチャＳＲＶ

public:
	bool Init(const char* _filename, const char* _vsfile, const char* _psfile, std::string _texfolder);
	void Uninit();
	void Update(
		unsigned int _animno,
		const DirectX::XMFLOAT4X4& _mtxworld);
	void Draw(DirectX::XMFLOAT4X4& _mtxworld);
	// ピクセルシェーダーセット
	bool SetPsShader(const char* _psfile);

	const ModelData& GetModelData() {
		return mAssimpFile;
	}
};
