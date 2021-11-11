#pragma once
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<d3d11.h>
#include	<vector>
#include	"../dx11/util/DX11util.h"
#include	"../dx11/Shader.h"

using Microsoft::WRL::ComPtr;

class Line {
private:
	ComPtr<ID3D11Buffer>		mVbuffer;			// ���_�o�b�t�@
	ComPtr<ID3D11PixelShader>	mPs;				// �s�N�Z���V�F�[�_�[
	ComPtr<ID3D11VertexShader>	mVs;				// ���_�V�F�[�_�[
	ComPtr<ID3D11InputLayout>   mLayout;			// ���_�t�H�[�}�b�g��`
	size_t						mVsize;			// ���_��
public:
	struct MyVertex {
		DirectX::XMFLOAT3		vertex;
		DirectX::XMFLOAT4		color;
	};

	Line() {

	}

	~Line() {

	}

	bool Init(const std::vector<MyVertex>& v) {
		mVsize = v.size();

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		unsigned int numElements = ARRAYSIZE(layout);

		// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
		bool sts = CreateVertexShader(
			GetDX11Device(),
			"shader/vsline.hlsl",
			"main",
			"vs_5_0",
			layout,
			numElements,
			&mVs,
			&mLayout);
		if (!sts)
		{
			MessageBox(nullptr, "CreateVertexShader(Line) error", "error", MB_OK);
			return false;
		}

		// �s�N�Z���V�F�[�_�[�𐶐�
		sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
								GetDX11Device(),				// �f�o�C�X�I�u�W�F�N�g
								"shader/psline.hlsl",
								"main",
								"ps_5_0",
								&mPs);
		if (!sts)
		{
			MessageBox(nullptr, "CreatePixelShader(Line) error", "error", MB_OK);
			return false;
		}

		// ���_�o�b�t�@����
		sts = CreateVertexBufferWrite(GetDX11Device(), static_cast<unsigned int>(sizeof(MyVertex)), static_cast<unsigned int>(v.size()), (void*)v.data(), &mVbuffer);
		if (!sts)
		{
			MessageBox(nullptr, "CreateVertexBufferWrite(Line) error", "error", MB_OK);
			return false;
		}

		return true;
	}

	void Dispose() {
	}

	void SetVertex(std::vector<MyVertex>& v) {

		D3D11_MAPPED_SUBRESOURCE pData;

		HRESULT hr = GetDX11DeviceContext()->Map(mVbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr))
		{
			memcpy_s(pData.pData, pData.RowPitch, (void*)(v.data()), sizeof(MyVertex) * v.size());
			GetDX11DeviceContext()->Unmap(mVbuffer.Get(), 0);
		}

		mVsize = v.size();
	}

	void Draw() {
		ID3D11DeviceContext* devcontext = GetDX11DeviceContext();

		// ���_�o�b�t�@���Z�b�g����
		unsigned int stride = sizeof(MyVertex);
		unsigned  offset = 0;
		devcontext->IASetVertexBuffers(0, 1, mVbuffer.GetAddressOf(), &stride, &offset);

		devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
		devcontext->IASetInputLayout(mLayout.Get());			// ���_���C�A�E�g�Z�b�g

		devcontext->VSSetShader(mVs.Get(), nullptr, 0);		// ���_�V�F�[�_�[���Z�b�g
		devcontext->PSSetShader(mPs.Get(), nullptr, 0);		// �s�N�Z���V�F�[�_�[���Z�b�g

		devcontext->Draw(
			static_cast<UINT>(mVsize),							// ���_��
			0);									// ���_�o�b�t�@�̍ŏ�����g��
	}
};