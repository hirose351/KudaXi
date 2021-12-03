#include	"dice_manager.h"
#include	"stagedata_manager.h"

void DiceManager::DiceCreate()
{
	for (int z = 0; z < mCurrentStageData.mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData.mMapSizeWidth; x++)
		{
			if (mCurrentStageData.mMap[x][z] > 0)
			{
				Dice* dice = new Dice;
				dice->GetTransform()->SetPosition(Float3(DICESCALE*x, 0, -DICESCALE * z));
				diceList.emplace_back(dice);
			}
		}
	}

}

void DiceManager::Init()
{
	DiceCreate();
}

void DiceManager::Update()
{
}

void DiceManager::Uninit()
{
	for (auto &obj : diceList)
	{
		if (obj != nullptr)
		{
			delete(obj);
		}
	}
}