#include	"dice_manager.h"
#include	"stagedata_manager.h"

void DiceManager::DiceCreate()
{
	int i = 0;
	std::string nameNum;
	for (int z = 0; z < mCurrentStageData.mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData.mMapSizeWidth; x++)
		{
			if (mCurrentStageData.mMap[z][x] > 0)
			{
				Dice* dice = new Dice;
				dice->GetTransform()->ReSetValue();
				dice->GetTransform()->SetPosition(Float3(DICE_SCALE*x, -DICE_SCALE / 2.0f, -DICE_SCALE * z));
				dice->GetTransform()->CreateMtx();
				dice->SetMapPos(INT3(x, 0, z));
				nameNum = ("Dice" + std::to_string(i));

				dice->SetName(nameNum);
				mDiceList.emplace_back(dice);
				i++;
			}
			mDiceMap[z][x] = mCurrentStageData.mMap[z][x];
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
	for (auto &obj : mDiceList)
	{
		if (obj != nullptr)
		{
			delete(obj);
		}
	}
}

bool DiceManager::CanDiceMove(Dice* _dice, Direction _dire)
{
	/// Todo:方向とINT3を連携させる何かを作る



	INT3 afterPos;
	switch (_dire)
	{
	case Direction::eUp:
		afterPos = { _dice->GetMapPos().x, 0, _dice->GetMapPos().z - 1 };
		break;
	case Direction::eDown:
		afterPos = { _dice->GetMapPos().x, 0, _dice->GetMapPos().z + 1 };
		break;
	case Direction::eLeft:
		afterPos = { _dice->GetMapPos().x - 1, 0, _dice->GetMapPos().z };
		break;
	case Direction::eRight:
		afterPos = { _dice->GetMapPos().x + 1, 0, _dice->GetMapPos().z };
		break;
	default:
		return false;
	}

	// 行き先が穴なら移動しない
	if (afterPos.z < 0 || afterPos.x < 0 || afterPos.z >= mCurrentStageData.mMapSizeHeight || afterPos.x >= mCurrentStageData.mMapSizeWidth)
		return false;
	// ステージ外なら移動しない
	if (mCurrentStageData.mFloorMap[afterPos.z][afterPos.x] <= 0)
		return false;
	// サイコロがあれば移動しない
	if (mDiceMap[afterPos.z][afterPos.x] > 0)
		return false;

	mDiceMap[afterPos.z][afterPos.x] = mDiceMap[_dice->GetMapPos().z][_dice->GetMapPos().x];
	mDiceMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = 0;
	_dice->SetMapPos(afterPos);
	return true;
}