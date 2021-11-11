#pragma once
#include	<d3d11.h>
#include	<wrl/client.h>

using Microsoft::WRL::ComPtr;

class CDirectXGraphics {
private:
	CDirectXGraphics() {}
	ComPtr<ID3D11Device>			mpDevice;					// DIRECT3DDEVICE11�f�o�C�X
	ComPtr<ID3D11DeviceContext>		mpImmediateContext;		// DIRECT3DDEVICE11�f�o�C�X�R���e�L�X�g
	ComPtr<IDXGISwapChain>			mpSwapChain;				// �X���b�v�`�F�C��

	ComPtr<ID3D11RenderTargetView>	mpRenderTargetView;		// �����_�[�^�[�Q�b�g�r���[
	ComPtr<ID3D11Texture2D>			mDepthStencilBuffer;		// �y�o�b�t�@�A�X�e���V���o�b�t�@
	ComPtr<ID3D11DepthStencilState>	mDepthStencilState;		// �y�o�b�t�@�A�X�e���V���X�e�[�g
	ComPtr<ID3D11DepthStencilView>	mDepthStencilView;			// �y�o�b�t�@�A�X�e���V���r���[
	ComPtr<ID3D11RasterizerState>	mRasterState;				// ���X�^�[�X�e�[�^�X

	D3D_DRIVER_TYPE					mDriverType;				// �h���C�o�^�C�v
	D3D_FEATURE_LEVEL				mFeatureLevel;				// �@�\���x��

	ComPtr<ID3D11BlendState>		mAlphaEnableBlendingState;		// �A���t�@�u�����h�X�e�[�g�i�L���j
	ComPtr<ID3D11BlendState>		mAlphaDisableBlendingState;	// �A���t�@�u�����h�X�e�[�g�i�����j

	ComPtr<ID3D11SamplerState>		mSamplerstate;					// �T���v���[�X�e�[�g

	int								mWidth = 0;				// �o�b�N�o�b�t�@�w�T�C�Y
	int								mHeight = 0;				// �o�b�N�o�b�t�@�x�T�C�Y
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

	// ��������
	bool Init(HWND hWnd, unsigned int Width, unsigned int Height, bool fullscreen);

	// �I������
	void Exit();

	// �f�o�C�XGET
	ID3D11Device*	GetDXDevice() const {
		return mpDevice.Get();
	}

	// �C�~�f�B�G�C�g�f�o�C�XGET
	ID3D11DeviceContext* GetImmediateContext() const {
		return mpImmediateContext.Get();
	}

	// SWAP�`�F�C��GET
	IDXGISwapChain* GetSwapChain()const {
		return mpSwapChain.Get();
	}

	// �����_�����O�^�[�Q�b�gGET
	ID3D11RenderTargetView* GetRenderTargetView() const {
		return mpRenderTargetView.Get();
	}

	// depthstencil view
	ID3D11DepthStencilView* GetDepthStencilView() const {
		return mDepthStencilView.Get();
	}

	// �r���[�|�[�g�̍������擾����
	int GetViewPortHeight() const {
		return mHeight;
	}

	// �r���[�|�[�g�̕����擾����
	int GetViewPortWidth() const {
		return mWidth;
	}

	// �A���t�@�u�����h�L����
	void TurnOnAlphaBlending();

	// �A���t�@�u�����h������
	void TurnOffAlphaBlending();

	// Z�o�b�t�@������
	void TurnOffZbuffer() {
		ID3D11RenderTargetView*  rtvtable[1];

		rtvtable[0] = mpRenderTargetView.Get();

		mpImmediateContext->OMSetRenderTargets(
			1,										// �^�[�Q�b�g
			rtvtable,								// �r���[�e�[�u��
			nullptr									// �[�x�o�b�t�@�Ȃ�
		);
	}

	// Z�o�b�t�@�L����
	void TurnOnZBuffer() {
		ID3D11RenderTargetView*  rtvtable[1];

		rtvtable[0] = mpRenderTargetView.Get();

		mpImmediateContext->OMSetRenderTargets(
			1,										// �^�[�Q�b�g
			rtvtable,								// �r���[�e�[�u��
			mDepthStencilView.Get()				// �[�x�o�b�t�@�Ȃ�
		);
	}
};
