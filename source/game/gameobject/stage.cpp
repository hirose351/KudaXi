#include	"stage.h"

void Stage::ObjectInit()
{
	/// Todo:現在のステージを取得する処理を加える

	mCurrentStageData.SetStageData(StageDataManager::GetInstance().GetCurrentStage());

	/// Todo:ゲーム開始時にカメラを移動できるように


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
