#pragma once
#include	<directxmath.h>
#include	"../../Memory.h"

using namespace DirectX;

class CCamera {
private:
	CCamera() {
	}
	XMFLOAT4X4		mProjection;

	XMFLOAT4X4		mCamera;

	XMFLOAT3		mEye;				// カメラ位置
	XMFLOAT3		mLookat;			// 注視点
	XMFLOAT3		mUp;				// 上向きベクトル

	float			mNear;
	float			mAspect;
	float			mFov;
	float			mFar;
public:
	CCamera(const CCamera&) = delete;
	CCamera& operator=(const CCamera&) = delete;
	CCamera(CCamera&&) = delete;
	CCamera& operator=(CCamera&&) = delete;

	static CCamera* GetInstance() {
		static CCamera instance;
		return &instance;
	}

	void Init(float nearclip, float farclip, float fov,
			  float width, float height,
			  XMFLOAT3 eye, XMFLOAT3 lookat, XMFLOAT3 up) {

		SetProjection(nearclip, farclip, fov, width, height);
		SetCamera(eye, lookat, up);
	}

	void SetNear(float nearclip) {
		mNear = nearclip;
	}

	void SetFar(float farclip) {
		mFar = farclip;
	}

	void SetFov(float fov) {
		mFov = fov;
	}

	void SetAspect(float width, float height) {
		mAspect = width / height;
	}

	void SetProjection(float nearclip, float farclip, float fov, float width, float height) {
		SetNear(nearclip);
		SetFar(farclip);
		SetFov(fov);
		SetAspect(width, height);
		CreateProjectionMatrix();
	}

	void SetCamera(const XMFLOAT3& eye, const XMFLOAT3& lookat, const XMFLOAT3& up) {

		SetEye(eye);
		SetLookat(lookat);
		SetUp(up);
		CreateCameraMatrix();
	}

	void SetEye(const XMFLOAT3& eye) {
		mEye = eye;
	}

	void SetLookat(const XMFLOAT3& lookat) {
		mLookat = lookat;
	}

	void SetUp(const XMFLOAT3& _up) {
		mUp = _up;
	}

	void CreateCameraMatrix() {
		ALIGN16 XMVECTOR Eye = XMVectorSet(mEye.x, mEye.y, mEye.z, 0.0f);
		ALIGN16 XMVECTOR At = XMVectorSet(mLookat.x, mLookat.y, mLookat.z, 0.0f);
		ALIGN16 XMVECTOR Up = XMVectorSet(mUp.x, mUp.y, mUp.z, 0.0f);

		ALIGN16 XMMATRIX camera;
		camera = XMMatrixLookAtLH(Eye, At, Up);

		XMStoreFloat4x4(&mCamera, camera);
	}

	void CreateProjectionMatrix() {

		ALIGN16 XMMATRIX projection;

		projection = XMMatrixPerspectiveFovLH(mFov, mAspect, mNear, mFar);

		XMStoreFloat4x4(&mProjection, projection);

	}

	const XMFLOAT4X4& GetCameraMatrix() {
		return mCamera;
	}

	const XMFLOAT4X4& GetProjectionMatrix() {
		return mProjection;
	}

	float GetFov() const {
		return mFov;
	}

	const XMFLOAT3& GetEye() const {
		return mEye;
	}

	const XMFLOAT3& GetLookat() const {
		return mLookat;
	}

	const XMFLOAT3& GetUp() const {
		return mUp;
	}

};