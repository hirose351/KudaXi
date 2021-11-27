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
	FILETYPE				mFileType = eASSIMPFILE;		// ファイルタイプ
	ModelData				mAssimpFile;					// assimpfile
	ID3D11VertexShader*     mpVertexShader = nullptr;		// 頂点シェーダー入れ物
	ID3D11PixelShader*      mpPixelShader = nullptr;		// ピクセルシェーダー入れ物
	ID3D11InputLayout*      mpVertexLayout = nullptr;		// 頂点フォーマット定義

	ID3D11ShaderResourceView*	mTexSRV = nullptr;			// テクスチャＳＲＶ

public:
	ModelComponent(const char * _fileName, const char * _vsFile, const char * _psFile, std::string _texFolder);

	//void Update(
		//unsigned int _animno,
		//const DirectX::XMFLOAT4X4& _mtxworld);

	void Draw()override;
	void Uninit() override;

	// ピクセルシェーダーセット
	bool SetPsShader(const char* _psfile);

	const ModelData& GetModelData() {
		return mAssimpFile;
	}
};