#include "camera.h"

using namespace DirectX;

void Camera::CreateCameraMatrix()
{
	ALIGN16 XMVECTOR Eye = XMVectorSet(mEye.x, mEye.y, mEye.z, 0.0f);
	ALIGN16 XMVECTOR At = XMVectorSet(mLookat.x, mLookat.y, mLookat.z, 0.0f);
	ALIGN16 XMVECTOR Up = XMVectorSet(mUp.x, mUp.y, mUp.z, 0.0f);

	ALIGN16 XMMATRIX camera;
	camera = XMMatrixLookAtLH(Eye, At, Up);

	XMStoreFloat4x4(&mCamera, camera);
}

void Camera::CreateProjectionMatrix()
{

	ALIGN16 XMMATRIX projection;

	projection = XMMatrixPerspectiveFovLH(mFov, mAspect, mNear, mFar);

	XMStoreFloat4x4(&mProjection, projection);

}