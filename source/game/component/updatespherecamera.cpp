#include	<DirectXMath.h>
#include	"CCamera.h"
#include	"../../system/dx11/util/dx11mathutil.h"

//  
// ‹…–Ê‘©”›ƒJƒƒ‰(‚yŽ²‰ñ“]‚È‚µƒo[ƒWƒ‡ƒ“)
//
void UpdateSphereCamera(
	float _radius,						// ”¼Œa(“ü—Í)
	float _elevation,					// ‹ÂŠpi“ü—Íj
	float _azimuth,						// •ûˆÊŠpi“ü—Íj
	const DirectX::XMFLOAT3& _lookat)	// ’Ž‹“_i“ü—Íj
{

	DirectX::XMFLOAT3 eye;				// Ž‹“_

	// ‹ÂŠpA•ûˆÊŠp‚ðƒ‰ƒWƒAƒ“‚É•ÏŠ·
	_elevation = ToRad(_elevation);
	_azimuth = ToRad(_azimuth);

	// ‹ÂŠp‚É‰ž‚¶‚½”¼Œa‚ðŒvŽZ
	float r = _radius * sinf(_elevation);

	// Ž‹“_À•W‚ðŒvŽZ
	eye.x = r * cosf(_azimuth);
	eye.y = _radius * cosf(_elevation);
	eye.z = r * sinf(_azimuth);

	// ãŒü‚«ƒxƒNƒgƒ‹‚ðŒvŽZ(‹ÂŠp‚ð‚ð‚X‚O“x‰ñ“]‚³‚¹‚é‚Æ‚Å‚«‚é)
	DirectX::XMFLOAT3 up;
	float elevationplus90 = _elevation + ToRad(90.0f);			// ‹ÂŠp‚ð‚X‚O“x‘«‚·

	r = _radius * sinf(elevationplus90);

	up.x = r * cosf(_azimuth);
	up.y = _radius * cosf(elevationplus90);
	up.z = r * sinf(_azimuth);

	// Ž‹“_ƒxƒNƒgƒ‹‚ð’Ž‹“_•ª•½sˆÚ“®
	eye.x += _lookat.x;
	eye.y += _lookat.y;
	eye.z += _lookat.z;

	// ƒrƒ…[•ÏŠ·s—ñ‚ðì¬‚·‚é
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetLookat(_lookat);
	CCamera::GetInstance()->SetUp(up);
	CCamera::GetInstance()->CreateCameraMatrix();
}
