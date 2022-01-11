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

	// 真っ白のSRVを一つ作っておく
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
	// 頂点バッファをセット
	devcon->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	// インデックスバッファをセット
	devcon->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// トポロジーをセット
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// SRVをセット
	if (mTextures.size() >= 1)
	{
		devcon->PSSetShaderResources(0, 1, &mTextures[0].texture);			// debug ts
		devcon->PSSetShaderResources(4, 1, &mToonSrv);			// debug ts
	}
	else
	{
		// 真っ白SRVをセット
		devcon->PSSetShaderResources(0, 1, &mWhitesrv);					// debug ts
	}
	// マテリアルをVSへセット
	devcon->VSSetConstantBuffers(3, 1, &mCbmtrl);
	devcon->PSSetConstantBuffers(3, 1, &mCbmtrl);

	// インデックスバッファを利用して描画
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
	// 頂点バッファ生成
	bool sts = CreateVertexBufferWrite(mDev,
									   static_cast<unsigned int>(sizeof(Vertex)),			// ストライド
									   static_cast<unsigned int>(mVertices.size()),		// 頂点数
									   mVertices.data(),									// 頂点データ
									   &mVertexBuffer);
	if (!sts)
	{
		return false;
	}

	// インデックスバッファ生成
	sts = CreateIndexBuffer(mDev,
							static_cast<unsigned int>(mIndices.size()),
							mIndices.data(),
							&mIndexBuffer);
	if (!sts)
	{
		return false;
	}

	// マテリアル用コンスタントバッファ生成
	sts = CreateConstantBufferWrite(mDev,
									sizeof(ConstantBufferMaterial),
									&mCbmtrl);
	if (!sts)
	{
		return false;
	}

	ID3D11DeviceContext* devicecontext;
	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// 定数バッファ更新
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
