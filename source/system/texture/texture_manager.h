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

public:
	static TextureManager& GetInstance() {
		static TextureManager Instance;
		return Instance;
	}

	virtual ~TextureManager() {
		Finalize();
	}

	void Finalize();

	// ロード
	bool LoadTexture(const std::string& _texFileName);

	// ポインタ取得
	TextureInfo* GetTexturePtr(std::string _key);
};