#pragma once
#include	"../util/uncopyable.h"
#include	"../dx11/CDirectxGraphics.h"
#include	<unordered_map>

//テクスチャ情報構造体
struct TextureInfo
{
	ComPtr <ID3D11Resource> texRes;
	ComPtr <ID3D11ShaderResourceView> texSrv;
};

class TextureManager : public Uncopyable
{
private:
	std::unordered_map<std::string, TextureInfo> mTextureInfo;	// テクスチャ格納辞書
	ID3D11Device* mpDevice;

	// ロード
	bool LoadTexture(std::string _texFileName);

	void Finalize();
public:
	static TextureManager& GetInstance() {
		static TextureManager Instance;
		return Instance;
	}

	TextureManager() {
		mpDevice = CDirectXGraphics::GetInstance()->GetDXDevice();
	}

	virtual ~TextureManager() {
		Finalize();
	}

	// ポインタ取得
	TextureInfo* GetTexturePtr(std::string _key);
};