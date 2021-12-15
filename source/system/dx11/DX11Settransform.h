#pragma once
#include	<DirectXMath.h>
#include	"Shader.h"
#include	"../../Memory.h"
#include	"CDirectxGraphics.h"

class DX11SetTransform {
private:
	DX11SetTransform() {}
public:
	DX11SetTransform(const DX11SetTransform&) = delete;
	DX11SetTransform& operator=(const DX11SetTransform&) = delete;
	DX11SetTransform(DX11SetTransform&&) = delete;
	DX11SetTransform& operator=(DX11SetTransform&&) = delete;

	static DX11SetTransform* GetInstance() {
		static DX11SetTransform instance;
		return &instance;
	}
	// 定数バッファ定義(ワールド変換行列)
	ALIGN16 struct ConstantBufferWorld
	{
		XMMATRIX World;				// ワールド変換行列
	};
	// 定数バッファ定義(ビュー変換行列)
	ALIGN16 struct ConstantBufferView
	{
		XMMATRIX View;				// ビュー変換行列
	};
	// 定数バッファ定義(毎フレーム設定)
	ALIGN16 struct ConstantBufferProjection
	{
		XMMATRIX Projection;		// プロジェクション変換行列
	};

	enum class TYPE {
		eWorld,
		eView,
		eProjection
	};

	void Uninit() {
		if (mpConstantBufferWorld)
		{
			mpConstantBufferWorld->Release();
			mpConstantBufferWorld = nullptr;
		}
		if (mpConstantBufferView)
		{
			mpConstantBufferView->Release();
			mpConstantBufferView = nullptr;
		}
		if (mpConstantBufferProjection)
		{
			mpConstantBufferProjection->Release();
			mpConstantBufferProjection = nullptr;
		}
	}

	bool Init() {

		ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();

		// コンスタントバッファ作成
		bool sts = CreateConstantBuffer(
			device,			// デバイス
			sizeof(ConstantBufferWorld),	// サイズ
			&mpConstantBufferWorld);			// コンスタントバッファ２
		if (!sts)
		{
			MessageBox(NULL, "CreateBuffer(constant buffer world) error", "Error", MB_OK);
			return false;
		}

		// コンスタントバッファ作成
		sts = CreateConstantBuffer(
			device,			// デバイス
			sizeof(ConstantBufferView),	// サイズ
			&mpConstantBufferView);			// コンスタントバッファ２
		if (!sts)
		{
			MessageBox(NULL, "CreateBuffer(constant buffer view) error", "Error", MB_OK);
			return false;
		}

		// コンスタントバッファ作成
		sts = CreateConstantBuffer(
			device,			// デバイス
			sizeof(ConstantBufferProjection),	// サイズ
			&mpConstantBufferProjection);			// コンスタントバッファ２
		if (!sts)
		{
			MessageBox(NULL, "CreateBuffer(constant buffer projection) error", "Error", MB_OK);
			return false;
		}
		return true;
	}

	void SetTransform(TYPE type, DirectX::XMFLOAT4X4& matrix) {
		ALIGN16 XMMATRIX mat;
		mat = DirectX::XMLoadFloat4x4(&matrix);

		ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

		switch (type)
		{
		case TYPE::eWorld:
			mCbWorld.World = XMMatrixTranspose(mat);
			devicecontext->UpdateSubresource(mpConstantBufferWorld, 0, nullptr, &mCbWorld, 0, 0);
			devicecontext->VSSetConstantBuffers(0, 1, &mpConstantBufferWorld);
			devicecontext->PSSetConstantBuffers(0, 1, &mpConstantBufferWorld);
			break;
		case TYPE::eView:
			mCbView.View = XMMatrixTranspose(mat);
			devicecontext->UpdateSubresource(mpConstantBufferView, 0, nullptr, &mCbView, 0, 0);
			devicecontext->VSSetConstantBuffers(1, 1, &mpConstantBufferView);
			devicecontext->PSSetConstantBuffers(1, 1, &mpConstantBufferView);
			break;
		case TYPE::eProjection:
			mCbProjection.Projection = XMMatrixTranspose(mat);
			devicecontext->UpdateSubresource(mpConstantBufferProjection, 0, nullptr, &mCbProjection, 0, 0);
			devicecontext->VSSetConstantBuffers(2, 1, &mpConstantBufferProjection);
			devicecontext->PSSetConstantBuffers(2, 1, &mpConstantBufferProjection);
			break;
		}
	}
private:
	ID3D11Buffer * mpConstantBufferWorld = nullptr;		// コンスタントバッファ2
	ID3D11Buffer * mpConstantBufferView = nullptr;			// コンスタントバッファ2
	ID3D11Buffer * mpConstantBufferProjection = nullptr;	// コンスタントバッファ2
	ConstantBufferWorld mCbWorld;				// b0 
	ConstantBufferView mCbView;				// b1
	ConstantBufferProjection mCbProjection;	// b2
};