#pragma once
#include	<DirectXMath.h>
#include	"memory.h"
#include	"../../system/dx11/Shader.h"
#include	"DX11util.h"

using namespace DirectX;

class CLight {
	ALIGN16 struct ConstantBufferLight {
		XMFLOAT4 LightDirection;
		XMFLOAT4 EyePos;
		XMFLOAT4 Ambient;
	};

	enum class LightType {
		DIRECTIONAL,
		POINT,
		SPOT
	};
	LightType			mType;
	XMFLOAT3			mEyepos;
	XMFLOAT4			mLightpos;		// w=0の時は方向 w=1の時は位置
	ID3D11Buffer*       mpConstantBufferLight = nullptr;
	XMFLOAT4			mAmbient;
public:
	bool Init(DirectX::XMFLOAT3 _eyepos, DirectX::XMFLOAT4 lightpos) {
		mLightpos = lightpos;
		mEyepos = _eyepos;
		mType = LightType::DIRECTIONAL;

		// コンスタントバッファ作成
		bool sts = CreateConstantBuffer(
			GetDX11Device(),				// デバイス
			sizeof(ConstantBufferLight),		// サイズ
			&mpConstantBufferLight);			// コンスタントバッファ４
		if (!sts)
		{
			MessageBox(NULL, "CreateBuffer(constant buffer Light) error", "Error", MB_OK);
			return false;
		}

		Update();

		return true;
	}

	void Update() {
		ConstantBufferLight		cb;

		cb.EyePos.w = 1.0;
		cb.EyePos.x = mEyepos.x;
		cb.EyePos.y = mEyepos.y;
		cb.EyePos.z = mEyepos.z;

		cb.LightDirection.x = mLightpos.x;
		cb.LightDirection.y = mLightpos.y;
		cb.LightDirection.z = mLightpos.z;
		cb.LightDirection.w = mLightpos.w;

		cb.Ambient = mAmbient;

		GetDX11DeviceContext()->UpdateSubresource(mpConstantBufferLight,
												  0,
												  nullptr,
												  &cb,
												  0, 0);

		// コンスタントバッファ4をｂ3レジスタへセット（頂点シェーダー用）
		GetDX11DeviceContext()->VSSetConstantBuffers(4, 1, &mpConstantBufferLight);
		// コンスタントバッファ4をｂ3レジスタへセット(ピクセルシェーダー用)
		GetDX11DeviceContext()->PSSetConstantBuffers(4, 1, &mpConstantBufferLight);

	}

	void Uninit() {
		if (mpConstantBufferLight)
		{
			mpConstantBufferLight->Release();
			mpConstantBufferLight = nullptr;
		}
	}

	void SetEyePos(const XMFLOAT3 &eyepos) {
		mEyepos = eyepos;
	}

	void SetLightPos(const XMFLOAT4 &lightpos) {
		mLightpos = lightpos;
	}

	void SetAmbient(const XMFLOAT4 &amb) {
		mAmbient = amb;
	}
};