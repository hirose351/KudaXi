#include	"access_camera_eye.h"
#include	"camera.h"
#include	"../component/easing_component.h"

CameraEyeAccess::CameraEyeAccess() :GameObject(("CameraEyeAccess"), ObjectType::eObstracle, false)
{
	AddComponent<Component::Easing>();
}

void CameraEyeAccess::ObjectInit()
{
	mTransform->SetPositionXYZ(Camera::GetInstance()->GetEye());
	mBeforePos = mTransform->position;
}

void CameraEyeAccess::ObjectUpdate()
{
	if (mBeforePos != mTransform->position)
	{
		Camera::GetInstance()->SetEye(mTransform->position);
		Camera::GetInstance()->CreateCameraMatrix();
	}
}

void CameraEyeAccess::ImguiCreateDraw()
{
}
