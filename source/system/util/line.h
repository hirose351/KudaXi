#pragma once
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<d3d11.h>
#include	<vector>
#include	"../dx11/DX11util.h"

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

	bool Init(const std::vector<MyVertex>& v);

	void Dispose() {}

	void SetVertex(std::vector<MyVertex>& v);

	void Draw();
};