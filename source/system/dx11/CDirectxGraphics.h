#pragma once
#include	<d3d11.h>
#include	<wrl/client.h>

using Microsoft::WRL::ComPtr;

class CDirectXGraphics {
private:
	CDirectXGraphics() {}
	ComPtr<ID3D11Device>			mpDevice;					// DIRECT3DDEVICE11デバイス
	ComPtr<ID3D11DeviceContext>		mpImmediateContext;		// DIRECT3DDEVICE11デバイスコンテキスト
	ComPtr<IDXGISwapChain>			mpSwapChain;				// スワップチェイン

	ComPtr<ID3D11RenderTargetView>	mpRenderTargetView;		// レンダーターゲットビュー
	ComPtr<ID3D11Texture2D>			mDepthStencilBuffer;		// Ｚバッファ、ステンシルバッファ
	ComPtr<ID3D11DepthStencilState>	mDepthStencilState;		// Ｚバッファ、ステンシルステート
	ComPtr<ID3D11DepthStencilView>	mDepthStencilView;			// Ｚバッファ、ステンシルビュー
	ComPtr<ID3D11RasterizerState>	mRasterState;				// ラスターステータス

	D3D_DRIVER_TYPE					mDriverType;				// ドライバタイプ
	D3D_FEATURE_LEVEL				mFeatureLevel;				// 機能レベル

	ComPtr<ID3D11BlendState>		mAlphaEnableBlendingState;		// アルファブレンドステート（有効）
	ComPtr<ID3D11BlendState>		mAlphaDisableBlendingState;	// アルファブレンドステート（無効）

	ComPtr<ID3D11SamplerState>		mSamplerstate;					// サンプラーステート

	int								mWidth = 0;				// バックバッファＸサイズ
	int								mHeight = 0;				// バックバッファＹサイズ
public:

	CDirectXGraphics(const CDirectXGraphics&) = delete;
	CDirectXGraphics& operator=(const CDirectXGraphics&) = delete;
	CDirectXGraphics(CDirectXGraphics&&) = delete;
	CDirectXGraphics& operator=(CDirectXGraphics&&) = delete;

	~CDirectXGraphics() {
		Exit();
	}

	static CDirectXGraphics* GetInstance() {
		static CDirectXGraphics instance;
		return &instance;
	}

	// 初期処理
	bool Init(HWND hWnd, unsigned int Width, unsigned int Height, bool fullscreen);

	// 終了処理
	void Exit();

	// デバイスGET
	ID3D11Device*	GetDXDevice() const {
		return mpDevice.Get();
	}

	// イミディエイトデバイスGET
	ID3D11DeviceContext* GetImmediateContext() const {
		return mpImmediateContext.Get();
	}

	// SWAPチェインGET
	IDXGISwapChain* GetSwapChain()const {
		return mpSwapChain.Get();
	}

	// レンダリングターゲットGET
	ID3D11RenderTargetView* GetRenderTargetView() const {
		return mpRenderTargetView.Get();
	}

	// depthstencil view
	ID3D11DepthStencilView* GetDepthStencilView() const {
		return mDepthStencilView.Get();
	}

	// ビューポートの高さを取得する
	int GetViewPortHeight() const {
		return mHeight;
	}

	// ビューポートの幅を取得する
	int GetViewPortWidth() const {
		return mWidth;
	}

	// アルファブレンド有効化
	void TurnOnAlphaBlending();

	// アルファブレンド無効化
	void TurnOffAlphaBlending();

	// Zバッファ無効化
	void TurnOffZbuffer() {
		ID3D11RenderTargetView*  rtvtable[1];

		rtvtable[0] = mpRenderTargetView.Get();

		mpImmediateContext->OMSetRenderTargets(
			1,										// ターゲット
			rtvtable,								// ビューテーブル
			nullptr									// 深度バッファなし
		);
	}

	// Zバッファ有効化
	void TurnOnZBuffer() {
		ID3D11RenderTargetView*  rtvtable[1];

		rtvtable[0] = mpRenderTargetView.Get();

		mpImmediateContext->OMSetRenderTargets(
			1,										// ターゲット
			rtvtable,								// ビューテーブル
			mDepthStencilView.Get()				// 深度バッファなし
		);
	}
};
