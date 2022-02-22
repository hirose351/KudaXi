#include	"sphere.h"
#include	"../dx11/dx11_mathutil.h"

// 法線ベクトルを計算,ノーマライズ
void Sphere::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal)
{
	DX11Vec3Normalize(Normal, Normal);
}

// インデックスデータを作成
void Sphere::CreateIndex()
{
	// インデックス生成
	for (unsigned int y = 0; y < mDivY; y++)
	{
		for (unsigned int x = 0; x < mDivX; x++)
		{
			int count = (mDivX + 1)*y + x;			// 左上座標のインデックス

			Face f;
			// 上半分
			f.idx[0] = count;
			f.idx[1] = count + 1;
			f.idx[2] = count + 1 + (mDivX + 1);

			mFace.emplace_back(f);

			// 下半分
			f.idx[0] = count;
			f.idx[1] = count + (mDivX + 1) + 1;
			f.idx[2] = count + (mDivX + 1);

			mFace.emplace_back(f);

		}
	}
}

// 頂点データを作成
void Sphere::CreateVertex()
{
	float azimuth = 0.0f;			// 方位角
	float elevation = 0.0f;			// 仰角

	XMFLOAT3	Normal;
	// 方位角と仰角から球メッシュの頂点データを作成
	for (unsigned int y = 0; y <= mDivY; y++)
	{
		elevation = (PI * (float)y) / (float)mDivY;			// 仰角をセット
		float r = mRadius * sinf(elevation);				// 仰角に応じた半径を計算

		for (unsigned int x = 0; x <= mDivX; x++)
		{
			azimuth = (2 * PI * (float)x) / (float)mDivX;	// 方位角をセット

			Vertex v;
			v.Pos.x = r * cosf(azimuth);
			v.Pos.y = mRadius * cosf(elevation);
			v.Pos.z = r * sinf(azimuth);

			Normalize(v.Pos, Normal);		// 法線を計算
			v.Normal = Normal;				// 法線をセット

			v.Color = mColor;				// 頂点カラー

			mVertex.emplace_back(v);
		}
	}
}

// 描画
void Sphere::Draw(ID3D11DeviceContext* _device)
{
	// 頂点バッファをセットする
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	_device->IASetVertexBuffers(0, 1, mpVertexBuffer.GetAddressOf(), &stride, &offset);

	_device->IASetIndexBuffer(mpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);	// インデックスバッファをセット
	_device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// トポロジーをセット（旧プリミティブタイプ）
	_device->IASetInputLayout(mpVertexLayout.Get());							// 頂点レイアウトセット

	_device->VSSetShader(mpVertexShader.Get(), nullptr, 0);			// 頂点シェーダーをセット
	_device->PSSetShader(mpPixelShader.Get(), nullptr, 0);			// ピクセルシェーダーをセット

	_device->DrawIndexed(
		static_cast<unsigned int>(mFace.size() * 3),		// 描画するインデックス数（面数×３）
		0,													// 最初のインデックスバッファの位置
		0);													// 頂点バッファの最初から使う
}

bool Sphere::Init(float _r,					// 半径
				  int _divisionHorizontal,	// 水平方向の分割数
				  int _divisionVertical,	// 垂直方向の分割数
				  XMFLOAT4 color,
				  ID3D11Device* device)
{
	bool sts;
	// 分割数を保存
	mDivX = _divisionHorizontal;
	mDivY = _divisionVertical;

	// 半径を保存
	mRadius = _r;

	// カラー値を保存
	mColor = color;

	// 球のインデックスデータを作成
	CreateIndex();

	// 球の頂点データを作成
	CreateVertex();

	// 頂点バッファ作成
	sts = CreateVertexBufferWrite(
		device,
		sizeof(Vertex),								// １頂点当たりバイト数
		static_cast<unsigned int>(mVertex.size()),	// 頂点数
		mVertex.data(),								// 頂点データ格納メモリ先頭アドレス
		mpVertexBuffer.GetAddressOf());				// 頂点バッファ
	if (!sts)
	{
		MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
		return false;
	}

	// インデックスバッファ作成
	sts = CreateIndexBuffer(
		device,											// デバイスオブジェクト
		static_cast<unsigned int>(mFace.size()) * 3,	// インデックス数
		mFace.data(),									// インデックスデータ先頭アドレス
		mpIndexBuffer.GetAddressOf());					// インデックスバッファ

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
	sts = CreatePixelShader(						// ピクセルシェーダーオブジェクトを生成
							device,					// デバイスオブジェクト
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
