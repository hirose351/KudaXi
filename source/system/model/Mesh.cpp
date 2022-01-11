#include	<string>
#include	<vector>
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<string>
#include	<unordered_map>
#include	"memory.h"
#include	"../dx11/CDirectxGraphics.h"
#include	"../dx11/Shader.h"
#include	"vertexproto.h"
#include	"Mesh.h"
#include	"../dx11/DX11util.h"

using namespace std;
using namespace DirectX;
using Microsoft::WRL::ComPtr;

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<MeshTextureInfo> textures, Material mtrl) {

	// �^������SRV�������Ă���
	bool sts = CreateSRVfromFile("assets/white.bmp", GetDX11Device(), GetDX11DeviceContext(), &mWhiteres, &mWhitesrv);
	if (!sts)
	{
		MessageBox(nullptr, "white,bmp load error", "error", MB_OK);
	}
	sts = CreateSRVfromFile("assets/shader/toon.png", GetDX11Device(), GetDX11DeviceContext(), &mToonRes, &mToonSrv);
	if (!sts)
	{
		MessageBox(nullptr, "toon.png load error", "error", MB_OK);
	}

	mVertices = vertices;
	mIndices = indices;
	mDev = CDirectXGraphics::GetInstance()->GetDXDevice();
	mTextures = textures;
	mMtrl = mtrl;
	SetupMesh();
}

void Mesh::Draw(ID3D11DeviceContext *devcon)
{
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	// ���_�o�b�t�@���Z�b�g
	devcon->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	// �C���f�b�N�X�o�b�t�@���Z�b�g
	devcon->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// �g�|���W�[���Z�b�g
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// SRV���Z�b�g
	if (mTextures.size() >= 1)
	{
		devcon->PSSetShaderResources(0, 1, &mTextures[0].texture);			// debug ts
		devcon->PSSetShaderResources(4, 1, &mToonSrv);			// debug ts
	}
	else
	{
		// �^����SRV���Z�b�g
		devcon->PSSetShaderResources(0, 1, &mWhitesrv);					// debug ts
	}
	// �}�e���A����VS�փZ�b�g
	devcon->VSSetConstantBuffers(3, 1, &mCbmtrl);
	devcon->PSSetConstantBuffers(3, 1, &mCbmtrl);

	// �C���f�b�N�X�o�b�t�@�𗘗p���ĕ`��
	devcon->DrawIndexed(static_cast<unsigned int>(mIndices.size()), 0, 0);
}

void Mesh::Close()
{
	mVertexBuffer->Release();
	mIndexBuffer->Release();
	mCbmtrl->Release();
}

bool Mesh::SetupMesh()
{
	// ���_�o�b�t�@����
	bool sts = CreateVertexBufferWrite(mDev,
									   static_cast<unsigned int>(sizeof(Vertex)),			// �X�g���C�h
									   static_cast<unsigned int>(mVertices.size()),		// ���_��
									   mVertices.data(),									// ���_�f�[�^
									   &mVertexBuffer);
	if (!sts)
	{
		return false;
	}

	// �C���f�b�N�X�o�b�t�@����
	sts = CreateIndexBuffer(mDev,
							static_cast<unsigned int>(mIndices.size()),
							mIndices.data(),
							&mIndexBuffer);
	if (!sts)
	{
		return false;
	}

	// �}�e���A���p�R���X�^���g�o�b�t�@����
	sts = CreateConstantBufferWrite(mDev,
									sizeof(ConstantBufferMaterial),
									&mCbmtrl);
	if (!sts)
	{
		return false;
	}

	ID3D11DeviceContext* devicecontext;
	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �萔�o�b�t�@�X�V
	D3D11_MAPPED_SUBRESOURCE pData;

	ConstantBufferMaterial cb;

	cb.AmbientMaterial.w = 1.0f;
	cb.AmbientMaterial.x = mMtrl.mAmbient.x;
	cb.AmbientMaterial.y = mMtrl.mAmbient.y;
	cb.AmbientMaterial.z = mMtrl.mAmbient.z;

	cb.DiffuseMaterial.w = 1.0f;
	cb.DiffuseMaterial.x = mMtrl.mDiffuse.x;
	cb.DiffuseMaterial.y = mMtrl.mDiffuse.y;
	cb.DiffuseMaterial.z = mMtrl.mDiffuse.z;

	cb.SpecularMaterial.w = 1.0f;
	cb.SpecularMaterial.x = mMtrl.mSpecular.x;
	cb.SpecularMaterial.y = mMtrl.mSpecular.y;
	cb.SpecularMaterial.z = mMtrl.mSpecular.z;

	HRESULT hr = devicecontext->Map(mCbmtrl, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr))
	{
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(ConstantBufferMaterial));
		devicecontext->Unmap(mCbmtrl, 0);
	}

	return true;
}
