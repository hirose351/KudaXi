#pragma once
#include	"../util/uncopyable.h"
#include	"../dx11/directx_graphics.h"
#include	<unordered_map>
#include	<DirectXMath.h>

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

	void Finalize();

public:
	static TextureManager& GetInstance()
	{
		static TextureManager Instance;
		return Instance;
	}

	TextureManager();

	~TextureManager()
	{
		Finalize();
	}

	// ロード
	bool LoadTexture(std::string _texFileName);

	// ポインタ取得
	TextureInfo* GetTexturePtr(std::string _key);
};