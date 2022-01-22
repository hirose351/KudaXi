#include	"stage.h"

Stage::Stage() :GameObject(("Stage"), ObjectType::eStage, false)
{
	AddComponent<Component::Plane>();
}

void Stage::ObjectInit()
{
	/// Todo:���݂̃X�e�[�W���擾���鏈����������

	mCurrentStageData = StageDataManager::GetInstance().GetCurrentStage();

	/// Todo:�Q�[���J�n���ɃJ�������ړ��ł���悤��

	Float3 mCameraLookat, mCameraEye;

	mCameraLookat.x = mCurrentStageData->mMapSizeWidth*DICE_SCALE_HALF;
	mCameraLookat.y = { 0 };
	mCameraLookat.z = { -mCurrentStageData->mMapSizeHeight*DICE_SCALE_HALF };
	Camera::GetInstance()->SetLookat(mCameraLookat);
	mCameraEye = { 140, 130, -170 };
	Camera::GetInstance()->SetEye(mCameraEye);
	Camera::GetInstance()->CreateCameraMatrix();
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