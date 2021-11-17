#pragma once
#include	"../util/uncopyable.h"
#include	"../dx11/CDirectxGraphics.h"
#include	<unordered_map>

//�e�N�X�`�����\����
struct TextureInfo
{
	ComPtr <ID3D11Resource> texRes;
	ComPtr <ID3D11ShaderResourceView> texSrv;
};

class TextureManager : public Uncopyable
{
private:
	std::unordered_map<std::string, TextureInfo> mTextureInfo;	// �e�N�X�`���i�[����

public:
	static TextureManager& GetInstance() {
		static TextureManager Instance;
		return Instance;
	}

	virtual ~TextureManager() {
		Finalize();
	}

	void Finalize();

	// ���[�h
	bool LoadTexture(const std::string& _texFileName);

	// �|�C���^�擾
	TextureInfo* GetTexturePtr(std::string _key);
};