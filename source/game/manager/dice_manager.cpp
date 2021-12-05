#include	"dice_manager.h"
#include	"stagedata_manager.h"
#include	"../../system/util/INT3.h"

void DiceManager::DiceCreate()
{
	for (int z = 0; z < mCurrentStageData.mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData.mMapSizeWidth; x++)
		{
			if (mCurrentStageData.mMap[z][x] > 0)
			{
				Dice* dice = new Dice;
				dice->GetTransform()->SetPosition(Float3(DICESCALE*x, DICESCALE / 2.0f, -DICESCALE * z));
				dice->GetTransform()->CreateMtx();
				dice->SetMapPos(INT3(x, 0, z));
				diceList.emplace_back(dice);
			}
			diceMap[z][x] = mCurrentStageData.mMap[z][x];
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

bool DiceManager::CanPush(Dice* _dice, DIRECTION _dire)
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

	// s‚«æ‚ªŒŠ‚È‚çˆÚ“®‚µ‚È‚¢
	if (mCurrentStageData.mFloorMap[afterPos.z][afterPos.x] <= 0)
		return false;


	if (diceMap[afterPos.z][afterPos.x] <= 0)
	{
		diceMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = -1;
		diceMap[afterPos.z][afterPos.x] = diceMap[_dice->GetMapPos().z][_dice->GetMapPos().x];
		_dice->SetMapPos(afterPos);
		return true;
	}

	return false;
}
