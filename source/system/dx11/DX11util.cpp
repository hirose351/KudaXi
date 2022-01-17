#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"CDirectxGraphics.h"
#include	"Shader.h"
#include	"../../game/gameobject/camera.h"
#include	"../../memory.h"
#include	"../../game/gameobject/CLight.h"
#include	"DX11Settransform.h"

CLight				gDirectionallight;

void DX11LightInit(DirectX::XMFLOAT4 lightpos) {
	gDirectionallight.Init(Camera::GetInstance()->GetEye(), lightpos);
	gDirectionallight.SetAmbient(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));				// ����
	gDirectionallight.Update();
}

void DX11LightUninit() {
	gDirectionallight.Uninit();
}

void DX11LightUpdate(DirectX::XMFLOAT4 lpos) {
	gDirectionallight.SetEyePos(Camera::GetInstance()->GetEye());
	gDirectionallight.SetLightPos(lpos);
	gDirectionallight.Update();
}

CDirectXGraphics* GetDX11Obj() {
	return CDirectXGraphics::GetInstance();
}

ID3D11Device* GetDX11Device() {
	return CDirectXGraphics::GetInstance()->GetDXDevice();
}

ID3D11DeviceContext*	GetDX11DeviceContext() {
	return CDirectXGraphics::GetInstance()->GetImmediateContext();
}

bool DX11Init(HWND hwnd, int width, int height, bool fullscreen) {
	bool sts;

	sts = CDirectXGraphics::GetInstance()->Init(hwnd, width, height, fullscreen);
	if (!sts)
	{
		MessageBox(hwnd, "DX11 init error", "error", MB_OK);
		return false;
	}

	sts = DX11SetTransform::GetInstance()->Init();
	if (!sts)
	{
		MessageBox(NULL, "SetTransform error", "Error", MB_OK);
		return false;
	}

	return true;
}

void DX11Uninit() {

	DX11SetTransform::GetInstance()->Uninit();

	// �������
	CDirectXGraphics::GetInstance()->Exit();

}

void DX11BeforeRender(float ClearColor[]) {
	// �^�[�Q�b�g�o�b�t�@�N���A
	CDirectXGraphics::GetInstance()->GetImmediateContext()->ClearRenderTargetView(
		CDirectXGraphics::GetInstance()->GetRenderTargetView(), ClearColor);
	// Z�o�b�t�@�N���A
	CDirectXGraphics::GetInstance()->GetImmediateContext()->ClearDepthStencilView(
		CDirectXGraphics::GetInstance()->GetDepthStencilView(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX11AfterRender() {
	// ��������
	CDirectXGraphics::GetInstance()->GetSwapChain()->Present(0, 0);
}

int DX11GetScreenWidth() {
	return CDirectXGraphics::GetInstance()->GetViewPortWidth();
}

int DX11GetScreenHeight() {
	return CDirectXGraphics::GetInstance()->GetViewPortHeight();
}

void TurnOffZbuffer() {
	CDirectXGraphics::GetInstance()->TurnOffZbuffer();
}

void TurnOnZbuffer() {
	CDirectXGraphics::GetInstance()->TurnOnZBuffer();
}

void TurnOnAlphablend() {
	CDirectXGraphics::GetInstance()->TurnOnAlphaBlending();
}

void TurnOffAlphablend() {
	CDirectXGraphics::GetInstance()->TurnOffAlphaBlending();
}

void TurnWire()
{
}

void TurnSolid()
{
}
