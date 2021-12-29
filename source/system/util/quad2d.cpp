#include	"quad2d.h"
#include	"../dx11/DX11Settransform.h"

// comptr
using Microsoft::WRL::ComPtr;

// 使用する頂点シェーダー名
const char* vsfilename[] = {
	"shader/basicvs.hlsl"
};
// 使用するピクセルシェーダー名
const char* psfilename[] = {
	"shader/basicps.hlsl",
	"shader/basicnotexps.hlsl"
};

// 矩形の初期化
bool Quad2D::Init(float width, float height, const char *tex_name, const DirectX::XMFLOAT4 &color, int _u, int _v, float z) {
	//DX11MtxIdentity(mWorldmtx);	// 初期姿勢

	// 4角形の初期化処理
	XMFLOAT2 uv[4] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{0.0f,1.0f},
		{1.0f,1.0f}
	};

	mWidth = width;
	mHeight = height;
	mColor = color;

	// ピクセルシェーダーを生成
	bool sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[0]);
	if (!sts)
	{
		MessageBox(nullptr, "SetPixelShader error(basicps.hlsl)", "error", MB_OK);
		return false;
	}

	// ピクセルシェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[1]);
	if (!sts)
	{
		MessageBox(nullptr, "SetPixelShader error(basicnotexps.hlsl)", "error", MB_OK);
		return false;
	}

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// エレメント数
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		vsfilename[0],			// 頂点シェーダーファイル名
		layout,					// 頂点レイアウト
		numElements);			// エレメント数

	// 頂点初期化
	UpdateVertex(width, height, color, z);

	// デバイスを取得
	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// 頂点バッファを生成
	sts = CreateVertexBufferWrite(
		device,						// デバイスオブジェクト
		sizeof(Quad2D::Vertex),		// １頂点当たりバイト数
		4,							// 頂点数
		mVertex,					// 頂点データ格納メモリ先頭アドレス
		&mVertexbuffer				// 頂点バッファ
	);
	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	// インデックスバッファを生成
	uint32_t index[4] = { 0,1,2,3 };

	sts = CreateIndexBuffer(
		device,						// デバイスオブジェクト
		4,							// インデックス数
		index,						// インデックス格納メモリ先頭アドレス
		&mIndexbuffer				// インデックスバッファ
	);
	if (!sts)
	{
		MessageBox(nullptr, "CreateIndexBuffer error", "error", MB_OK);
		return false;
	}

	// テクスチャ設定
	device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	sts = CreateSRVfromFile(tex_name, device, devicecontext, &mTexInfo.texRes, &mTexInfo.texSrv);
	if (!sts)
	{
		// テクスチャ不要な場合はNoTexと入力して描画させないでおく
		MessageBox(nullptr, "CreateSRVfromfile エラー", "error!!", MB_OK);
		return false;
	}

	// アニメーション設定
	mDivu = _u;				// 横分割数を入れる
	mDivv = _v;				// 縦分割数を入れる

	SetTextureUV(0, 0);
	UpdateVertex(width, height, color);
	UpdateVbuffer();

	return true;
}

// 描画
void Quad2D::Draw(DirectX::XMFLOAT4X4 _mtx) {
	// デバイスコンテキストを取得
	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// 座標変換用の行列をセット
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eWorld, _mtx);
	//DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eWorld, mWorldmtx);

	unsigned int stride = sizeof(Quad2D::Vertex);	// ストライドをセット（１頂点当たりのバイト数）
	unsigned  offset = 0;						// オフセット値をセット

	// デバイスコンテキストをシェーダーリソースへセット
	devcontext->PSSetShaderResources(0, 1, mTexInfo.texSrv.GetAddressOf());

	// 頂点バッファをデバイスコンテキストへセット
	devcontext->IASetVertexBuffers(
		0,									// スタートスロット
		1,									// 頂点バッファ個数
		mVertexbuffer.GetAddressOf(),		// 頂点バッファの先頭アドレス
		&stride,							// ストライド
		&offset);							// オフセット

	// インデックスバッファをデバイスコンテキストへセット
	devcontext->IASetIndexBuffer(
		mIndexbuffer.Get(),				// インデックスバッファ
		DXGI_FORMAT_R32_UINT,				// フォーマット
		0);									// オフセット

	// トポロジーをセット
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点シェーダー、ピクセルシェーダー取得
	ID3D11VertexShader* vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
	ID3D11PixelShader* ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

	// 頂点レイアウト取得
	ID3D11InputLayout* layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);

	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// 頂点フォーマットをセット
	devcontext->IASetInputLayout(layout);

	// ドローコール発行
	devcontext->DrawIndexed(
		4,						// インデックス数
		0,						// 開始インデックス
		0);						// 基準頂点インデックス

	//TextureManager::GetInstance().Draw(mTexInfo, mWorldmtx);
}

// 描画
void Quad2D::DrawNoTex() {
	//// デバイスコンテキストを取得
	//ID3D11DeviceContext* devcontext;
	//devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	//// 座標変換用の行列をセット
	//DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mWorldmtx);

	//unsigned int stride = sizeof(Quad2D::Vertex);	// ストライドをセット（１頂点当たりのバイト数）
	//unsigned  offset = 0;						// オフセット値をセット

	//// 頂点バッファをデバイスコンテキストへセット
	//devcontext->IASetVertexBuffers(
	//	0,									// スタートスロット
	//	1,									// 頂点バッファ個数
	//	mVertexbuffer.GetAddressOf(),		// 頂点バッファの先頭アドレス
	//	&stride,							// ストライド
	//	&offset);							// オフセット

	//// インデックスバッファをデバイスコンテキストへセット
	//devcontext->IASetIndexBuffer(
	//	mIndexbuffer.Get(),				// インデックスバッファ
	//	DXGI_FORMAT_R32_UINT,				// フォーマット
	//	0);									// オフセット

	//// トポロジーをセット
	//devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//// 頂点シェーダー、ピクセルシェーダー取得
	//ID3D11VertexShader* vs;
	//vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
	//ID3D11PixelShader* ps;
	//ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[1]);

	//// 頂点レイアウト取得
	//ID3D11InputLayout* layout;
	//layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);

	//devcontext->VSSetShader(vs, nullptr, 0);
	//devcontext->GSSetShader(nullptr, nullptr, 0);
	//devcontext->HSSetShader(nullptr, nullptr, 0);
	//devcontext->DSSetShader(nullptr, nullptr, 0);
	//devcontext->PSSetShader(ps, nullptr, 0);

	//// 頂点フォーマットをセット
	//devcontext->IASetInputLayout(layout);

	//// ドローコール発行
	//devcontext->DrawIndexed(
	//	4,						// インデックス数
	//	0,						// 開始インデックス
	//	0);						// 基準頂点インデックス
}
//
//// 拡大、縮小
//void Quad2D::SetScale(Float3 _size) {
//	DX11MtxScale(_size.x, _size.y, _size.z, mWorldmtx);
//}
//
//// 位置をセット
//void Quad2D::SetPosition(Float3 _pos) {
//	mWorldmtx._41 = _pos.x;
//	mWorldmtx._42 = _pos.y;
//	mWorldmtx._43 = _pos.z;
//}

void Quad2D::SetColor(const DirectX::XMFLOAT4 &_color)
{
	mColor = _color;

	Quad2D::Vertex	v[4] = {
		// 座標													// カラー	// UV	
		XMFLOAT3(-mWidth / 2.0f,	-mHeight / 2.0f, 0.0f),	_color,		mUv[0],
		XMFLOAT3(mWidth / 2.0f,	-mHeight / 2.0f, 0.0f),	_color,		mUv[1],
		XMFLOAT3(-mWidth / 2.0f,	 mHeight / 2.0f, 0.0f),	_color,		mUv[2],
		XMFLOAT3(mWidth / 2.0f,	 mHeight / 2.0f, 0.0f),	_color,		mUv[3]
	};

	for (int i = 0; i < 4; i++)
	{
		mVertex[i] = v[i];
	}
}

//// Z軸回転
//void Quad2D::SetZRotation(float angle) {
//	DX11MtxRotationZ(angle, mWorldmtx);
//}
//
//// X軸回転
//void Quad2D::SetXRotation(float angle) {
//	DX11MtxRotationX(angle, mWorldmtx);
//}

// 頂点データ更新
void Quad2D::UpdateVertex(uint32_t width, uint32_t height, const DirectX::XMFLOAT4 &color, float z) {

	mWidth = static_cast<float>(width);
	mHeight = static_cast<float>(height);

	Quad2D::Vertex	v[4] = {
		// 座標													// カラー	// UV	
		XMFLOAT3(-mWidth / 2.0f,	-mHeight / 2.0f, z),	color,		mUv[0],
		XMFLOAT3(mWidth / 2.0f,	-mHeight / 2.0f, z),	color,		mUv[1],
		XMFLOAT3(-mWidth / 2.0f,	 mHeight / 2.0f, z),	color,		mUv[2],
		XMFLOAT3(mWidth / 2.0f,	 mHeight / 2.0f, z),	color,		mUv[3]
	};

	for (int i = 0; i < 4; i++)
	{
		mVertex[i] = v[i];
	}
}

// 頂点バッファ更新
void Quad2D::UpdateVbuffer() {
	D3D11_MAPPED_SUBRESOURCE pData;
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	HRESULT hr = devcontext->Map(mVertexbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr))
	{
		memcpy_s(pData.pData, pData.RowPitch, (void*)(mVertex), sizeof(Quad2D::Vertex) * 4);
		devcontext->Unmap(mVertexbuffer.Get(), 0);
	}
}


// UV座標設定　(_u：左から何番目か  _v：上から何番目か　direction：描画方向(０→左 １→右)　デフォルトは右)
void Quad2D::SetTextureUV(int _u, int _v, int direction) {

	// 左向き
	if (direction == 0)
	{
		mUv[0] = { 1.0f / mDivu * (_u + 1), 1.0f / mDivv * _v };
		mUv[1] = { 1.0f / mDivu * _u,	     1.0f / mDivv * _v };
		mUv[2] = { 1.0f / mDivu * (_u + 1), 1.0f / mDivv * (_v + 1) };
		mUv[3] = { 1.0f / mDivu * _u,	     1.0f / mDivv * (_v + 1) };
	}
	// 右向き
	else
	{
		mUv[0] = { 1.0f / mDivu * _u,	     1.0f / mDivv * _v };
		mUv[1] = { 1.0f / mDivu * (_u + 1), 1.0f / mDivv * _v };
		mUv[2] = { 1.0f / mDivu * _u,	     1.0f / mDivv * (_v + 1) };
		mUv[3] = { 1.0f / mDivu * (_u + 1), 1.0f / mDivv * (_v + 1) };
	}
	UpdateVertex((uint32_t)mWidth, (uint32_t)mHeight, mColor);
	UpdateVbuffer();
}