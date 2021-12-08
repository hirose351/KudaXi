#include	"stage.h"

void Stage::ObjectInit()
{
	/// Todo:���݂̃X�e�[�W���擾���鏈����������

	mCurrentStageData.SetStageData(StageDataManager::GetInstance().GetCurrentStage());

	/// Todo:�Q�[���J�n���ɃJ�������ړ��ł���悤��


	mPlane.Init();
}

void Stage::ObjectDraw()
{
	for (int z = 0; z < mCurrentStageData.mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData.mMapSizeWidth; x++)
		{
			if (mCurrentStageData.mFloorMap[z][x] != 0)
			{
				mPlane.GetTransform()->SetPosition(Float3(DICE_SCALE*x, 0, -DICE_SCALE * z));
				mPlane.Draw();
			}
		}
	}
}
