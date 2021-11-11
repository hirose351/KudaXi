#include	<DirectXMath.h>
#include	"CCamera.h"
#include	"../../system/dx11/util/dx11mathutil.h"

//  
// ���ʑ����J����(�y����]�Ȃ��o�[�W����)
//
void UpdateSphereCamera(
	float _radius,						// ���a(����)
	float _elevation,					// �p�i���́j
	float _azimuth,						// ���ʊp�i���́j
	const DirectX::XMFLOAT3& _lookat)	// �����_�i���́j
{

	DirectX::XMFLOAT3 eye;				// ���_

	// �p�A���ʊp�����W�A���ɕϊ�
	_elevation = ToRad(_elevation);
	_azimuth = ToRad(_azimuth);

	// �p�ɉ��������a���v�Z
	float r = _radius * sinf(_elevation);

	// ���_���W���v�Z
	eye.x = r * cosf(_azimuth);
	eye.y = _radius * cosf(_elevation);
	eye.z = r * sinf(_azimuth);

	// ������x�N�g�����v�Z(�p�����X�O�x��]������Ƃł���)
	DirectX::XMFLOAT3 up;
	float elevationplus90 = _elevation + ToRad(90.0f);			// �p���X�O�x����

	r = _radius * sinf(elevationplus90);

	up.x = r * cosf(_azimuth);
	up.y = _radius * cosf(elevationplus90);
	up.z = r * sinf(_azimuth);

	// ���_�x�N�g���𒍎��_�����s�ړ�
	eye.x += _lookat.x;
	eye.y += _lookat.y;
	eye.z += _lookat.z;

	// �r���[�ϊ��s����쐬����
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetLookat(_lookat);
	CCamera::GetInstance()->SetUp(up);
	CCamera::GetInstance()->CreateCameraMatrix();
}
