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
	ID3D11Device* mpDevice;

	// ���[�h
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

	// �|�C���^�擾
	TextureInfo* GetTexturePtr(std::string _key);
};