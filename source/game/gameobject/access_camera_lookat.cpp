#include	"access_camera_lookat.h"
#include	"camera.h"
#include	"../component/easing_component.h"

CameraAccessLookat::CameraAccessLookat() :GameObject(("CameraAccessLookat"), ObjectType::eObstracle, false)
{
	AddComponent<Component::Easing>();
}

void CameraAccessLookat::ObjectInit()
{
	mTransform->SetPositionXYZ(Camera::GetInstance()->GetLookat());
	mBeforePos = mTransform->position;
}

void CameraAccessLookat::ObjectUpdate()
{
	if (mBeforePos != mTransform->position)
	{
		Camera::GetInstance()->SetLookat(mTransform->position);
		Camera::GetInstance()->CreateCameraMatrix();
	}
}

void CameraAccessLookat::ImguiCreateDraw()
{
}
