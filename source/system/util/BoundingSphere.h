//*****************************************************************************
//!	@file	CBoundingSphere.h
//!	@brief	境界球クラス
//!	@note	
//!	@author
//*****************************************************************************
#pragma once

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<DirectXMath.h>
#include	"Sphere.h"
#include	"../dx11/dx11mathutil.h"
#include	"../dx11/DX11util.h"
#include	"../dx11/DX11Settransform.h"

//-----------------------------------------------------------------------------
// マクロ宣言
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
struct	HITDATA {
	float	minx, miny, minz;			// バウンディングボックス最小値	
	float	maxx, maxy, maxz;			// バウンディングボックス最大値
	float	cx, cy, cz;					// バウンディングスフィア中心座標
	float	r;							// バウンディングスフィア半径
};

class BoundingSphere {
private:
	HITDATA				mHitData;				// 当たり判定用データ
	DirectX::XMFLOAT3	mWpos;					// ワールド空間上の位置座標
	DirectX::XMFLOAT4X4	mMatrix;				// 境界球の描画用行列
	Sphere				mSphereMesh;			// 球メッシュ

	void CalculateBoundingSphere(std::vector<XMFLOAT3>& vertices);
	void CreateSphereMesh(float r, DirectX::XMFLOAT4 color) {
		// バウンディングスフィアを表す球メッシュを生成
		bool sts = mSphereMesh.Init(r,				// 半径
									50,								// 水平方向の分割数
									50,								// 垂直方向の分割数
									color,
									GetDX11Device());				// デバイスオブジェクト
		if (!sts)
		{
			MessageBox(nullptr, "CSphere init error", "error", MB_OK);
		}
	}
public:
	BoundingSphere() {
	}

	~BoundingSphere() {
		Exit();
	}

	void Init(std::vector<XMFLOAT3>& vertices, XMFLOAT4 color) {
		CalculateBoundingSphere(vertices);			// ヒットデータの計算
		CreateSphereMesh(mHitData.r, color);		// 球メッシュを作成する
	}

	void UpdatePosition(const DirectX::XMFLOAT4X4& mat) {
		DirectX::XMFLOAT3 pos(mHitData.cx, mHitData.cy, mHitData.cz);

		// 行列×ベクトル（ワールド座標に変換）
		DX11Vec3MulMatrix(mWpos, pos, mat);

		mMatrix = mat;
		mMatrix._41 = mWpos.x;
		mMatrix._42 = mWpos.y;
		mMatrix._43 = mWpos.z;
	}

	void Exit() {
	}

	void Draw() {
		ID3D11DeviceContext* device;
		device = GetDX11DeviceContext();
		// ワールド変換行列
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eWorld, mMatrix);
		mSphereMesh.Draw(device);
	}

	float GetR() const {
		return mHitData.r;
	}

	const DirectX::XMFLOAT3& GetWPos() {
		return mWpos;
	}

	const HITDATA& GetHitdata() {
		return mHitData;
	}
};

//******************************************************************************
//	End of file.
//******************************************************************************