#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	"../dx11/CDirectxGraphics.h"
#include	"../dx11/Shader.h"
#include	"2dsystem.h"

using Microsoft::WRL::ComPtr;

ComPtr<ID3D11Buffer> gpBviewPort;
ComPtr<ID3D11Buffer> gpBworld;

bool Init2D() {

	ID3D11Device* device;
	ID3D11DeviceContext* devicecontext;

	// �f�o�C�X�擾
	device = CDirectXGraphics::GetInstance()->GetDXDevice();
	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();


	// �萔�o�b�t�@�𐶐�
	bool sts = CreateConstantBufferWrite(device, sizeof(ConstantBufferViewPort), &gpBviewPort);
	if (!sts)
	{
		MessageBox(nullptr, "CreateConstantBuffer error viewport", "error", MB_OK);
		return false;
	}

	// �萔�o�b�t�@�𐶐�
	sts = CreateConstantBuffer(device, sizeof(ConstantBufferWorld), &gpBworld);
	if (!sts)
	{
		MessageBox(nullptr, "CreateConstantBuffer error world", "error", MB_OK);
		return false;
	}

	// �萔�o�b�t�@�X�V
	ConstantBufferViewPort cb;
	cb.ViewportHeight[0] = CDirectXGraphics::GetInstance()->GetViewPortHeight();
	cb.ViewportWidth[0] = CDirectXGraphics::GetInstance()->GetViewPortWidth();

	D3D11_MAPPED_SUBRESOURCE pData;
	HRESULT hr = devicecontext->Map(gpBviewPort.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr))
	{
		memcpy_s(pData.pData, pData.RowPitch, (void*)&cb, sizeof(ConstantBufferViewPort));
		devicecontext->Unmap(gpBviewPort.Get(), 0);
	}

	// �萔�o�b�t�@���f�o�t�փZ�b�g
	devicecontext->VSSetConstantBuffers(5, 1, gpBviewPort.GetAddressOf());

	return false;
}

void Exit2D() {

}