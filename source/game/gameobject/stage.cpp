#include	"stage.h"

Stage::Stage() :GameObject(("Stage"), ObjectType::eObstracle, false)
{
	mpPlane = AddComponent<Component::Plane>();
}

void Stage::ObjectInit()
{
	/// Todo:���݂̃X�e�[�W���擾���鏈����������

	mCurrentStageData.SetStageData(StageDataManager::GetInstance().GetCurrentStage());

	/// Todo:�Q�[���J�n���ɃJ�������ړ��ł���悤��

}

void Stage::ObjectUpdate()
{
	for (int z = 0; z < mCurrentStageData.mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData.mMapSizeWidth; x++)
		{
			if (mCurrentStageData.mFloorMap[z][x] != 0)
			{
				mpPlane->SetDrawPos(Float3(DICE_SCALE*x, 0, -DICE_SCALE * z));
			}
		}
	}
}