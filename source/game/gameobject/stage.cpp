#include	"stage.h"

void Stage::CameraUpdate()
{
	Float3 mCameraLookat, mCameraEye;
	Float3 cameraVector(20.5f, 4.4f, -27.5f);

	mCameraLookat.x = mCurrentStageData->mMapSizeWidth*DICE_SCALE_HALF;
	mCameraLookat.y = { 0 };
	mCameraLookat.z = { -mCurrentStageData->mMapSizeHeight*DICE_SCALE_HALF };

	mCameraEye.x = cameraVector.x * mCurrentStageData->mMapSizeWidth;
	mCameraEye.y = 70 + cameraVector.y * (mCurrentStageData->mMapSizeWidth + mCurrentStageData->mMapSizeHeight);
	mCameraEye.z = cameraVector.z * mCurrentStageData->mMapSizeHeight;

	Camera::GetInstance()->SetLookat(mCameraLookat);
	Camera::GetInstance()->SetEye(mCameraEye);
	Camera::GetInstance()->CreateCameraMatrix();
}

Stage::Stage() :GameObject(("Stage"), ObjectType::eStage, false)
{
	AddComponent<Component::Plane>();
}

void Stage::ObjectInit()
{
	/// Todo:現在のステージを取得する処理を加える

	mCurrentStageData = StageDataManager::GetInstance().GetCurrentStage();

	for (int x = 0; x < 10; x++)
		for (int z = 0; z < 10; z++)
			mCurrentStageData->mFloorMap[x][z] = 1;

	/// Todo:ゲーム開始時にカメラを移動できるように
	//CameraUpdate();
}

void Stage::ObjectUpdate()
{
	for (int z = 0; z < mCurrentStageData->mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData->mMapSizeWidth; x++)
		{
			if (mCurrentStageData->mFloorMap[z][x] != 0)
			{
				GetComponent<Component::Plane>()->SetDrawPos(Float3(DICE_SCALE*x, 0, -DICE_SCALE * z));
			}
		}
	}
}

void Stage::ImguiCreateDraw()
{
	ImGui::Text("Size");
	ImGui::SliderInt("Width", &mCurrentStageData->mMapSizeHeight, 3, 10);
	ImGui::SliderInt("Height", &mCurrentStageData->mMapSizeWidth, 3, 10);
	CameraUpdate();
}
