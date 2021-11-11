#pragma once

#include	"../dx11/CDirectxGraphics.h"
#include	<DirectXMath.h>
#include	<unordered_map>
#include	<string>

using namespace DirectX;

class CBillboard
{
private:
	//�e�N�X�`�����\����
	struct TextureInfo
	{
		ComPtr <ID3D11Resource> texRes;
		ComPtr <ID3D11ShaderResourceView> texSrv;
	};
protected:
	//�萔�o�b�t�@�\����
	struct CB_DATA
	{
		float r, g, b, a;			//�J���[
		float u0, v0, u3, v3;		//�e�N�X�`��
	};

	static ComPtr <ID3D11InputLayout> smIL;		//���̓��C�A�E�g
	static ComPtr <ID3D11VertexShader> smVS;	//���_�V�F�[�_�[
	static ComPtr <ID3D11PixelShader> smPS;		//�s�N�Z���V�F�[�_�[

	static std::unordered_map<std::string, TextureInfo> smTextureInfo;		//�e�N�X�`���i�[����

	XMFLOAT3 mPos;								//�|�W�V����

	XMFLOAT2 mScale, mUV0, mUV3, mDivUv;		//�X�P�[��,UV���
	XMFLOAT4 mColor;							//�J���[

	XMFLOAT4X4 mMatrixWorld;					//���[���h�ϊ��s��

	ComPtr <ID3D11Buffer> mpVetexBuffer = nullptr;				//���_�o�b�t�@

	ComPtr <ID3D11Resource> mpTexRes = nullptr;					//�e�N�X�`�����
	ComPtr <ID3D11ShaderResourceView> mpTexSRV = nullptr;

	ComPtr <ID3D11Buffer> mpConstantBuffer = nullptr;			//�萔�o�b�t�@

	bool mShouldErase = false;		//�����ׂ���


public:
	CBillboard();
	virtual ~CBillboard() = default;

	HRESULT Initialize(void);
	HRESULT LoadTexture(const std::string pTexFileName);

	virtual void Update(void);
	virtual void Render(void);

	static HRESULT CreateShader(void);

	const XMFLOAT3& GetPos()const {
		return mPos;
	}

	const XMFLOAT4& GetColor()const {
		return mColor;
	}

	const bool& GetShouldErase()const {
		return mShouldErase;
	}

	void SetPos(XMFLOAT3 pos)
	{
		mPos = pos;
	}

	void SetScale(XMFLOAT2 scale)
	{
		mScale = scale;
	}

	void SetColor(XMFLOAT4 color)
	{
		mColor = color;
	}

	void SetDivUV(XMFLOAT2 _divuv)
	{
		mDivUv = _divuv;
	}

	void SetUV(XMFLOAT2 _uv/*, int _direction*/)
	{
		mUV0.x = 1.0f / mDivUv.x * _uv.x;
		mUV0.y = 1.0f / mDivUv.y * _uv.y;
		mUV3.x = 1.0f / mDivUv.x * (_uv.x + 1);
		mUV3.y = 1.0f / mDivUv.y * (_uv.y + 1);
	}

	void SetRotation(XMFLOAT4X4 _mtx);
};