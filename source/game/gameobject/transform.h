#pragma once
#include	"../../system/util/vector.h"

struct Transform
{
	//protected:
	Float3 position;		// 位置
	Float3 move;			// 移動量
	Float3 angle;			// 回転
	Float3 rotation;		// 回転量
	Float3 scale;			// 大きさ
	DirectX::XMFLOAT4X4 worldMtx;	// ワールド行列
	DirectX::XMFLOAT4X4 localMtx;	// ローカル行列
	DirectX::XMFLOAT4X4 scaleMtx;	// 拡大行列

	Transform();

	// Todo:回転とか移動とかの関数作る

	// 初期値に戻す
	void ReSetValue();
	// 移動量加算
	void AddPosition();
	// 拡大行列作成
	void CreateScaleMtx();
	// 行列を作成
	void CreateMtx();

	void SetPosition(const Float3& _pos) { position = _pos; }
	void SetRotation(const Float3& _rot) { rotation = _rot; }
	void SetScale(const Float3& _sc) { scale = _sc; }
	void SetAngle(const Float3& _ang) { angle = _ang; }
	void SetMtx(const DirectX::XMFLOAT4X4 _mtx) { worldMtx = _mtx; }
	Float3 GetPosition() { return position; }
	Float3 GetRotation() { return rotation; }
	Float3 GetScale() { return scale; }
	Float3 GetAngle() { return angle; }
	DirectX::XMFLOAT4X4 GetMtx() { return worldMtx; }
};