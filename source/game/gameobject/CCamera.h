#pragma once
#include	<directxmath.h>
#include	"../../Memory.h"
#include	"../../system/util/uncopyable.h"
#include	"../../system/util/vector.h"

class CCamera :Uncopyable {
private:
	DirectX::XMFLOAT4X4		mProjection;
	DirectX::XMFLOAT4X4		mCamera;

	DirectX::XMFLOAT3		mEye;				// カメラ位置
	DirectX::XMFLOAT3		mLookat;			// 注視点
	DirectX::XMFLOAT3		mUp;				// 上向きベクトル

	float			mNear;
	float			mAspect;
	float			mFov;
	float			mFar;
public:
	static CCamera* GetInstance() {
		static CCamera instance;
		return &instance;
	}

	void Init(float nearclip, float farclip, float fov, float width, float height,
			  DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 lookat, DirectX::XMFLOAT3 up) {

		SetProjection(nearclip, farclip, fov, width, height);
		SetCamera(eye, lookat, up);
	}

	void CreateCameraMatrix();
	void CreateProjectionMatrix();

	void SetNear(float nearclip) { mNear = nearclip; }
	void SetFar(float farclip) { mFar = farclip; }
	void SetFov(float fov) { mFov = fov; }
	void SetAspect(float width, float height) { mAspect = width / height; }

	void SetProjection(float nearclip, float farclip, float fov, float width, float height) {
		SetNear(nearclip);
		SetFar(farclip);
		SetFov(fov);
		SetAspect(width, height);
		CreateProjectionMatrix();
	}

	void SetCamera(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& lookat, const DirectX::XMFLOAT3& up) {

		SetEye(eye);
		SetLookat(lookat);
		SetUp(up);
		CreateCameraMatrix();
	}

	void SetEye(const DirectX::XMFLOAT3& eye) { mEye = eye; }
	void SetLookat(const DirectX::XMFLOAT3& lookat) { mLookat = lookat; }
	void SetUp(const DirectX::XMFLOAT3& _up) { mUp = _up; }

	float GetFov() const { return mFov; }
	const DirectX::XMFLOAT4X4& GetCameraMatrix() { return mCamera; }
	const DirectX::XMFLOAT4X4& GetProjectionMatrix() { return mProjection; }
	const DirectX::XMFLOAT3& GetEye() const { return mEye; }
	const DirectX::XMFLOAT3& GetLookat() const { return mLookat; }
	const DirectX::XMFLOAT3& GetUp() const { return mUp; }
};