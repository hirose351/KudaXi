#pragma once
#include	<DirectXMath.h>

void UpdateSphereCamera(
	float _radius,							// ���a(����)
	float _elevation,						// �p�i���́j
	float _azimuth,							// ���ʊp�i���́j
	const DirectX::XMFLOAT3& _lookat);		// �����_�i���́j