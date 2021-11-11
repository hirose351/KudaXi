#pragma once
#include	<DirectXMath.h>

void UpdateSphereCamera(
	float _radius,							// 半径(入力)
	float _elevation,						// 仰角（入力）
	float _azimuth,							// 方位角（入力）
	const DirectX::XMFLOAT3& _lookat);		// 注視点（入力）