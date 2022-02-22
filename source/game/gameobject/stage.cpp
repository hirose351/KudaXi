#include	"stage.h"
#include	"../manager/stagedata_manager.h"
#include	"../component/plane.h"

void Stage::CameraUpdate()
{
	Float3 mCameraLookat, mCameraEye, cameraVector(20.5f, 4.4f, -27.5f);

	mCameraLookat.x = mpCurrentStageData->mMapSizeWidth*DICE_SCALE_HALF;
	mCameraLookat.y = { 0 };
	mCameraLookat.z = { -mpCurrentStageData->mMapSizeHeight*DICE_SCALE_HALF };

	mCameraEye.x = cameraVector.x * mpCurrentStageData->mMapSizeWidth;
	mCameraEye.y = 70 + cameraVector.y * (mpCurrentStageData->mMapSizeWidth + mpCurrentStageData->mMapSizeHeight);
	mCameraEye.z = cameraVector.z * mpCurrentStageData->mMapSizeHeight;

	Camera::GetInstance()->SetLookat(mCameraLookat);
	Camera::GetInstance()->SetEye(mCameraEye);
	Camera::GetInstance()->CreateCameraMatrix();
}

void Stage::Reset()
{
	mpCurrentStageData = StageDataManager::GetInstance().GetCurrentStage();

	for (int x = 0; x < 10; x++)
		for (int z = 0; z < 10; z++)
			mpCurrentStageData->mFloorMap[x][z] = 1;

	mpCurrentStageData->mMapSizeWidth = mpCurrentStageData->mMapSizeHeight = 3;
}

Stage::Stage() :GameObject(("Stage"), ObjectType::eStage, false)
{
	AddComponent<Component::Plane>();
}

void Stage::ObjectInit()
{
	mpCurrentStageData = StageDataManager::GetInstance().GetCurrentStage();

	for (int x = 0; x < 10; x++)
		for (int z = 0; z < 10; z++)
			mpCurrentStageData->mFloorMap[x][z] = 1;
}

void Stage::ObjectUpdate()
{
	for (int z = 0; z < mpCurrentStageData->mMapSizeHeight; z++)
	{
		for (int x = 0; x < mpCurrentStageData->mMapSizeWidth; x++)
		{
			if (mpCurrentStageData->mFloorMap[z][x] != 0)
			{
				GetComponent<Component::Plane>()->SetDrawPos(Float3(DICE_SCALE*x, 0, -DICE_SCALE * z));
			}
		}
	}
}

void Stage::ImguiCreateDraw()
{
	ImGui::Text("Size");
	ImGui::SliderInt("Width", &mpCurrentStageData->mMapSizeWidth, 3, 10);
	ImGui::SliderInt("Height", &mpCurrentStageData->mMapSizeHeight, 3, 10);
	CameraUpdate();
}
