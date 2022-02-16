#pragma once

#include	"../dx11/CDirectxGraphics.h"
#include	<DirectXMath.h>
#include	<unordered_map>
#include	<string>

class CBillboard
{
private:
	// テクスチャ情報構造体
	struct TextureInfo
	{
		ComPtr <ID3D11Resource> texRes;
		ComPtr <ID3D11ShaderResourceView> texSrv;
	};

protected:
	// 定数バッファ構造体
	struct CB_DATA
	{
		float r, g, b, a;			// カラー
		float u0, v0, u3, v3;		// テクスチャ
	};

	static ComPtr <ID3D11InputLayout> smIL;		// 入力レイアウト
	static ComPtr <ID3D11VertexShader> smVS;	// 頂点シェーダー
	static ComPtr <ID3D11PixelShader> smPS;		// ピクセルシェーダー

	static std::unordered_map<std::string, TextureInfo> smTextureInfo;		// テクスチャ格納辞書

	DirectX::XMFLOAT2 mScale, mUV0, mUV3, mDivUv;				// スケール,UV情報
	DirectX::XMFLOAT4 mColor;									// カラー

	DirectX::XMFLOAT4X4 mMatrixWorld;							// ワールド変換行列

	ComPtr <ID3D11Buffer> mpVetexBuffer = nullptr;				// 頂点バッファ
	ComPtr <ID3D11Resource> mpTexRes = nullptr;					// テクスチャ情報
	ComPtr <ID3D11ShaderResourceView> mpTexSRV = nullptr;
	ComPtr <ID3D11Buffer> mpConstantBuffer = nullptr;			// 定数バッファ

	bool mShouldErase = false;									// 消すべきか

public:
	CBillboard();
	virtual ~CBillboard() = default;

	HRESULT Initialize(void);
	HRESULT LoadTexture(const std::string pTexFileName);

	virtual void Update(DirectX::XMFLOAT4X4 _mtx);
	virtual void Render(void);

	static HRESULT CreateShader(void);

	/// アクセサ ////////////////////////////////////////////////

	const DirectX::XMFLOAT4& GetColor()const
	{
		return mColor;
	}

	const bool& GetShouldErase()const
	{
		return mShouldErase;
	}

	void SetScale(const DirectX::XMFLOAT2& scale)
	{
		mScale = scale;
	}

	void SetColor(const DirectX::XMFLOAT4& color)
	{
		mColor = color;
	}

	void SetDivUV(const DirectX::XMFLOAT2& _divuv)
	{
		mDivUv = _divuv;
	}

	void SetUV(const DirectX::XMFLOAT2& _uv)
	{
		mUV0.x = 1.0f / mDivUv.x * _uv.x;
		mUV0.y = 1.0f / mDivUv.y * _uv.y;
		mUV3.x = 1.0f / mDivUv.x * (_uv.x + 1);
		mUV3.y = 1.0f / mDivUv.y * (_uv.y + 1);
	}

	void SetRotation(DirectX::XMFLOAT4X4 _mtx);
};