#include	"dice_manager.h"
#include	"stagedata_manager.h"
#include	"../../system/util/INT3.h"

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
				dice->SetMapPos(INT3(x, 0, z));
				diceList.emplace_back(dice);
			}
			diceMap[x][z] = mCurrentStageData.mMap[x][z];
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

bool DiceManager::CheckMove(Dice* _dice, DIRECTION _dire)
{
	/// Todo:•ûŒü‚ÆINT3‚ğ˜AŒg‚³‚¹‚é‰½‚©‚ğì‚é

	INT3 afterPos;
	switch (_dire)
	{
	case DIRECTION::UP:
		afterPos = (_dice->GetMapPos() - INT3(0, 0, 1));
		break;
	case DIRECTION::DOWN:
		afterPos = (_dice->GetMapPos() + INT3(0, 0, 1));
		break;
	case DIRECTION::LEFT:
		afterPos = (_dice->GetMapPos() - INT3(1, 0, 0));
		break;
	case DIRECTION::RIGHT:
		afterPos = (_dice->GetMapPos() + INT3(1, 0, 0));
		break;
	default:
		return false;
	}

	// s‚«æ‚ªŒŠ‚È‚ç‰ñ“]‚µ‚È‚¢
	if (mCurrentStageData.mFloorMap[afterPos.x][afterPos.z] <= 0)
		return false;


	if (diceMap[afterPos.x][afterPos.z] <= 0)
	{
		diceMap[_dice->GetMapPos().x][_dice->GetMapPos().z] = -1;
		diceMap[afterPos.x][afterPos.z] = diceMap[_dice->GetMapPos().x][_dice->GetMapPos().z];
		_dice->SetMapPos(afterPos);
		return true;
	}

	return false;
}
