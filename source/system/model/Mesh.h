#pragma once
#include	<vector>
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<string>
#include	<wrl/client.h>
#include	"vertexproto.h"

// テクスチャ構造体
struct Texture {
	std::string type;							// テクスチャ種類
	std::string path;							// テクスチャパス名
	ID3D11Resource*	texres;						// テクスチャリソース
	ID3D11ShaderResourceView *texture;			// SRV
};

using Microsoft::WRL::ComPtr;

class Mesh {
public:
	ID3D11ShaderResourceView*    mWhitesrv;	// 真っ白ＳＲＶ
	ID3D11Resource*				mWhiteres;	// 真っ白RES

	std::vector<Vertex> mVertices;			// 頂点データ
	std::vector<unsigned int> mIndices;	// インデックス
	std::vector<Texture> mTextures;		// テクスチャ
	ID3D11Device* mDev;					// デバイス	
	Material mMtrl;						// マテリアル

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material mtrl);

	void Draw(ID3D11DeviceContext *devcon);
	void Close();
private:
	ID3D11Buffer*  mVertexBuffer = nullptr;	// 頂点バッファ
	ID3D11Buffer*  mIndexBuffer = nullptr;		// インデックスバッファ
	ID3D11Buffer*  mCbmtrl = nullptr;			// マテリアル用コンスタントバッファ

	bool SetupMesh();
};