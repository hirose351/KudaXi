#pragma once
#include	<directxmath.h>

// 頂点
struct Vertex {
	DirectX::XMFLOAT3 mPos;		// 位置
	DirectX::XMFLOAT3 mNormal;		// 法線
	DirectX::XMFLOAT2 mTex;		// UV座標
};

// マテリアル
struct Material {
	float mAlpha;
	DirectX::XMFLOAT3 mDiffuse;
	DirectX::XMFLOAT3 mSpecular;
	float	mSpecularPower;
	DirectX::XMFLOAT3 mAmbient;
	DirectX::XMFLOAT3 mEmissive;

	Material()
		: mAlpha(0),
		mDiffuse(DirectX::XMFLOAT3(0, 0, 0)),
		mSpecular(DirectX::XMFLOAT3(0, 0, 0)),
		mSpecularPower(0),
		mAmbient(DirectX::XMFLOAT3(0, 0, 0)),
		mEmissive(DirectX::XMFLOAT3(0, 0, 0))
	{
	}
};

// 定数バッファ定義（マテリアル）
struct ConstantBufferMaterial {
	DirectX::XMFLOAT4	AmbientMaterial;		// 環境光の反射率
	DirectX::XMFLOAT4	DiffuseMaterial;		// ディフューズ光の反射率
	DirectX::XMFLOAT4	SpecularMaterial;		// スペキュラ光の反射率
};