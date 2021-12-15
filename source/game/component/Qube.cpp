#include	"Qube.h"
#include	"../../system/dx11/dx11mathutil.h"
#include	"../../system/dx11/DX11util.h"
#include	"../../system/dx11/DX11Settransform.h"
#include	"../../system/dx11/Shader.h"

// 法線ベクトルを計算
// ノーマライズ
void Qube::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal) {
	DX11Vec3Normalize(Normal, Normal);
}

// インデックスデータを作成
void Qube::CreateIndex() {

	int vsidx[36] = { 2, 0, 1, 1, 3, 2, 4, 0, 2, 2, 6, 4,5, 1, 0, 0, 4, 5, 7, 3, 1, 1, 5, 7, 6, 2, 3, 3, 7, 6, 4, 6, 7, 7, 5, 4 };

	// インデックス生成
	for (unsigned int i = 0; i < 12; i++)
	{
		Face f;
		// 上半分
		f.idx[0] = vsidx[i * 3];
		f.idx[1] = vsidx[i * 3 + 1];
		f.idx[2] = vsidx[i * 3 + 2];
		mFace.emplace_back(f);
	}
}

// 頂点データを作成
void Qube::CreateVertex() {

	// 各頂点を設定
	mVertex[0].Pos = { -mAabb.hl.x, mAabb.hl.y, mAabb.hl.z };
	mVertex[1].Pos = { mAabb.hl.x, mAabb.hl.y,mAabb.hl.z };
	mVertex[2].Pos = { -mAabb.hl.x, mAabb.hl.y, -mAabb.hl.z };
	mVertex[3].Pos = { mAabb.hl.x, mAabb.hl.y, -mAabb.hl.z };
	mVertex[4].Pos = { -mAabb.hl.x, -mAabb.hl.y, mAabb.hl.z };
	mVertex[5].Pos = { mAabb.hl.x, -mAabb.hl.y, mAabb.hl.z };
	mVertex[6].Pos = { -mAabb.hl.x, -mAabb.hl.y, -mAabb.hl.z };
	mVertex[7].Pos = { mAabb.hl.x, -mAabb.hl.y, -mAabb.hl.z };

	for (int i = 0; i < 8; i++)
	{
		mVertex[i].Normal = mAabb.p;			// 法線をセット
		mVertex[i].Color = mColor;				// 頂点カラー
	}
}

void Qube::SetColor(const XMFLOAT4& _color)
{
	mColor = _color;
	if (mVertex.empty())
		return;
	for (auto &v : mVertex)
	{
		v.Color = mColor;				// 頂点カラー
	}
	ID3D11Device* device = GetDX11Device();
	// 頂点バッファ作成
	bool sts = CreateVertexBufferWrite(
		device,
		sizeof(Vertex),						// １頂点当たりバイト数
		static_cast<unsigned int>(mVertex.size()),					// 頂点数
		mVertex.data(),					// 頂点データ格納メモリ先頭アドレス
		mpVertexBuffer.GetAddressOf());	// 頂点バッファ
	if (!sts)
	{
		MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
	}

}

bool Qube::Init(const Primitive::AABB& _aabb, const XMFLOAT4& color) {
	ID3D11Device* device = GetDX11Device();
	bool sts;

	// 頂点データを作成
	for (int i = 0; i < 8; i++)
	{
		Vertex v;
		mVertex.emplace_back(v);
	}
	mAabb = _aabb;

	// カラー値を保存
	mColor = color;

	// 球のインデックスデータを作成
	CreateIndex();

	// 球の頂点データを作成
	CreateVertex();

	// 頂点バッファ作成
	sts = CreateVertexBufferWrite(
		device,
		sizeof(Vertex),						// １頂点当たりバイト数
		static_cast<unsigned int>(mVertex.size()),					// 頂点数
		mVertex.data(),					// 頂点データ格納メモリ先頭アドレス
		mpVertexBuffer.GetAddressOf());	// 頂点バッファ
	if (!sts)
	{
		MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
		return false;
	}

	// インデックスバッファ作成
	sts = CreateIndexBuffer(
		device,								// デバイスオブジェクト
		static_cast<unsigned int>(mFace.size()) * 3,					// インデックス数
		mFace.data(),						// インデックスデータ先頭アドレス
		mpIndexBuffer.GetAddressOf());		// インデックスバッファ

	if (!sts)
	{
		MessageBox(NULL, "CreateBuffer(index buffer) error", "Error", MB_OK);
		return false;
	}

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	sts = CreateVertexShader(device,
							 "shader/vssphere.hlsl",
							 "main",
							 "vs_5_0",
							 layout,
							 numElements,
							 &mpVertexShader,
							 &mpVertexLayout);
	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return false;
	}

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
							device,							// デバイスオブジェクト
							"shader/pssphere.hlsl",
							"main",
							"ps_5_0",
							&mpPixelShader);
	if (!sts)
	{
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return false;
	}

	return true;
}

void Qube::Update(const Primitive::AABB& _aabb)
{
	mAabb = _aabb;
	CreateVertex();
}

// 描画
void Qube::Draw(DirectX::XMFLOAT4X4 _mtx) {
	ID3D11DeviceContext* deviceContext = GetDX11DeviceContext();

	// 頂点バッファをセットする
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, mpVertexBuffer.GetAddressOf(), &stride, &offset);

	deviceContext->IASetIndexBuffer(mpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);			// インデックスバッファをセット
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// トポロジーをセット（旧プリミティブタイプ）
	deviceContext->IASetInputLayout(mpVertexLayout.Get());					// 頂点レイアウトセット

	deviceContext->VSSetShader(mpVertexShader.Get(), nullptr, 0);			// 頂点シェーダーをセット
	deviceContext->PSSetShader(mpPixelShader.Get(), nullptr, 0);			// ピクセルシェーダーをセット

	// ワールド変換行列セット
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eWorld, _mtx);

	deviceContext->DrawIndexed(
		static_cast<unsigned int>(mFace.size() * 3),		// 描画するインデックス数（面数×３）
		0,									// 最初のインデックスバッファの位置
		0);									// 頂点バッファの最初から使う
}

