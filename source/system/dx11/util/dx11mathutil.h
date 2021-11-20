#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include "../../../game/gameobject/CCamera.h"
#include "../CDirectxGraphics.h"

struct PLANE {
	float a, b, c, d;
};

struct INT3 {
	int x, y, z;
	INT3() = default;

	INT3(const INT3&) = default;
	INT3& operator=(const INT3&) = default;

	INT3(INT3&&) = default;
	INT3& operator=(INT3&&) = default;

	XM_CONSTEXPR INT3(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
	explicit INT3(_In_reads_(3) const int *pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
};

void DX11MakeWorldMatrix(DirectX::XMFLOAT4X4& mat, const DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT3& trans);
void DX11Vec3MulMatrix(DirectX::XMFLOAT3& outvec, const DirectX::XMFLOAT3& invec, const DirectX::XMFLOAT4X4& mat);
void DX11Vec3Cross(DirectX::XMFLOAT3& outvec, DirectX::XMFLOAT3& invec1, DirectX::XMFLOAT3& invec2);
void DX11Vec3Dot(float& dot, const DirectX::XMFLOAT3& invec1, const DirectX::XMFLOAT3& invec2);
void DX11Vec3Normalize(DirectX::XMFLOAT3& outvec, DirectX::XMFLOAT3& invec);
void DX11MtxIdentity(DirectX::XMFLOAT4X4& mat);
void DX11GetQtfromMatrix(const DirectX::XMFLOAT4X4& mat, DirectX::XMFLOAT4& qt);
void DX11QtRotationAxis(DirectX::XMFLOAT4& qt, DirectX::XMFLOAT4& axis, float angle);
void DX11QtMul(DirectX::XMFLOAT4& outqt, const DirectX::XMFLOAT4& qt1, const DirectX::XMFLOAT4& qt2);
void DX11MtxFromQt(DirectX::XMFLOAT4X4& outmtx, const DirectX::XMFLOAT4& qt);
void DX11QtNormalize(DirectX::XMFLOAT4& inqt, DirectX::XMFLOAT4& outqt);
void DX11Vec4Length(DirectX::XMFLOAT4& vec, float& length);
void DX11p2pLength(const DirectX::XMFLOAT3& p1, const DirectX::XMFLOAT3& p2, float& length);
void DX11MtxTranslation(const DirectX::XMFLOAT3& trans, DirectX::XMFLOAT4X4& matrix);
void DX11QtSlerp(const DirectX::XMFLOAT4& fromqt, const DirectX::XMFLOAT4& toqt, float t, DirectX::XMFLOAT4& ansqt);
void DX11QtIdentity(DirectX::XMFLOAT4& qt);
void DX11Vec3Length(DirectX::XMFLOAT3& vec, float& length);
void DX11MtxRotationAxis(const DirectX::XMFLOAT3& axis, float angle, DirectX::XMFLOAT4X4& outmtx);
void DX11MtxMultiply(DirectX::XMFLOAT4X4& ansmtx, const DirectX::XMFLOAT4X4& p1mtx, const DirectX::XMFLOAT4X4& p2mtx);
bool IsInFrustum(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT4X4& matrix);
void IsInFrustum(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT4X4& matrix, float ans[]);
bool IsInFrustum(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT4X4& matrix, float ans[], PLANE* plane);
bool IsInFrustum(
	const DirectX::XMFLOAT3& pos,			// BS中心座標
	float radius,							// 半径
	const DirectX::XMFLOAT4X4& matrix,		// ビュー変換行列＊プロジェクション変換行列 
	float ans[],							// 平面との距離
	bool  canseeflag[],						// フラスタム平面とＢＳの関係
	PLANE* plane);							// 平面方程式;
void GetPlanefromProjectionmatrix(PLANE* p, const DirectX::XMFLOAT4X4& matrix);
void GetPlanefromProjectionmatrixWithNormalize(PLANE* p, const DirectX::XMFLOAT4X4& matrix);
void DX11MtxInverse(DirectX::XMFLOAT4X4& ansmtx, const DirectX::XMFLOAT4X4& mtx);
void DX11MtxTranspose(DirectX::XMFLOAT4X4& outmtx, const DirectX::XMFLOAT4X4& inmtx);
void DX11MtxRotationX(float angle, DirectX::XMFLOAT4X4& outmtx);
void DX11MtxRotationY(float angle, DirectX::XMFLOAT4X4& outmtx);
void DX11MtxRotationZ(float angle, DirectX::XMFLOAT4X4& outmtx);
void DX11MtxScale(float sx, float sy, float sz, DirectX::XMFLOAT4X4& outmtx);
float haltonseq(unsigned int no, unsigned int base);
void transScreenToWorld(DirectX::XMFLOAT3*, int sx, int sy, float z);
void MtxSetPos(DirectX::XMFLOAT4X4& _mtx, DirectX::XMFLOAT3 _pos);
void MtxAddPos(DirectX::XMFLOAT4X4& _mtx, DirectX::XMFLOAT3 _pos);
//チェック対象と値を幅と比較対象地を引き算してその絶対値が一定の範囲内かどうかをチェック
bool floatcheck(float checvalue, float value, float minimum);
//ラジアンに変換する
float ToRad(float degree);