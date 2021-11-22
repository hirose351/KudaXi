#pragma once
#include	"../util/uncopyable.h"
#include	"../dx11/CDirectxGraphics.h"
#include	<unordered_map>
#include	<DirectXMath.h>

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


	void Finalize();
public:
	static TextureManager& GetInstance() {
		static TextureManager Instance;
		return Instance;
	}

	TextureManager();

	virtual ~TextureManager() {
		Finalize();
	}

	// ���[�h
	bool LoadTexture(std::string _texFileName);

	void Draw(TextureInfo _info, DirectX::XMFLOAT4X4 _mtx);

	// �|�C���^�擾
	TextureInfo* GetTexturePtr(std::string _key);
};