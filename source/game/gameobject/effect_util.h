#pragma once
#include	"../../system/dx11/dx11_util.h"

struct PARTICLE {
	int		uv;						// UV番号
	int		lifetime;				// 生存時間
	float	cameradistance;			// 距離
	DirectX::XMFLOAT3	pos;		// 位置
	DirectX::XMFLOAT3	velocity;	// 速度
	float	gravity;				// 重力
	float	height;					// 地面
	float	attenuation;			// 減衰
};

// 大小比較用の関数オブジェクト
struct ParticleLess {
	bool operator()(const PARTICLE& _a, const PARTICLE& _b) const noexcept {
		// キーとして比較したい要素を列挙する
		return _a.cameradistance > _b.cameradistance;
	}
};