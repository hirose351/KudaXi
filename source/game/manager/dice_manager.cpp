#include	"dice_manager.h"
#include	"stagedata_manager.h"

void DiceManager::DiceCreate()
{
	mCurrentStageData.SetStageData(StageDataManager::GetInstance().GetCurrentStage());
	std::string nameNum;
	for (int z = 0; z < mCurrentStageData.mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData.mMapSizeWidth; x++)
		{
			if (mCurrentStageData.mMap[z][x] < 0)
			{
				mDiceMap[z][x] = -1;
				continue;
			}
			// 生成配列に入れる
			mDiceMap[z][x] = static_cast<int>(mDiceList.size());
			Dice* dice = new Dice;
			dice->GetTransform()->ReSetValue();
			dice->GetTransform()->SetWordMtx(mCurrentStageData.mDiceMtx[mDiceList.size()]);


			dice->GetTransform()->SetScale(1.0f / 7.0f);
			dice->GetTransform()->CreateScaleMtx();

			dice->GetTransform()->SetPositionMove(Float3(DICE_SCALE*x, -DICE_SCALE_HALF, -DICE_SCALE * z));
			//dice->GetTransform()->CreateMtx();
			dice->SetMapPos(INT3(x, 0, z));
			// オブジェクトの名前に添え字を加える
			nameNum = ("Dice" + std::to_string(mDiceMap[z][x]));
			dice->SetName(nameNum);
			// vector配列に追加
			mDiceList.emplace_back(dice);
			//mDiceMap[z][x] = mCurrentStageData.mMap[z][x];
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

	//// サイコロがあれば移動しない
	//if (mDiceMap[afterPos.z][afterPos.x] > -1)
	//	return false;

	// サイコロがあるとき
	if (mDiceMap[afterPos.z][afterPos.x] > -1)
	{
		DICESTATUS dSts = mDiceList[mDiceMap[afterPos.z][afterPos.x]]->GetDiceStatus();
		// 半分以上存在しているサイコロなら移動しない
		if (dSts == DICESTATUS::NORMAL || dSts == DICESTATUS::DOWN || dSts == DICESTATUS::HALF_UP)
			return false;
		// 半分以下のサイコロならそのサイコロを消す
		//SetRemoveDice(mDiceList[mDiceMap[afterPos.z][afterPos.x]]);
	}

	mDiceMap[afterPos.z][afterPos.x] = mDiceMap[_dice->GetMapPos().z][_dice->GetMapPos().x];
	mDiceMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = -1;
	_dice->SetMapPos(afterPos);
	return true;
}

bool DiceManager::CanDiceMoveCheak(Dice * _dice, Direction _dire)
{
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
		return true;
	// ステージ外なら移動しない
	if (mCurrentStageData.mFloorMap[afterPos.z][afterPos.x] <= 0)
		return true;
	return false;
}

void DiceManager::CheckAligned(Dice* _dice)
{
	if (_dice->GetTopDiceTypeNum() == 1)
		return;

	// 現在のブロック位置をチェック用配列にコピー
	memcpy((void *)mCheckMap, (void *)mDiceMap, sizeof(mDiceMap));

	// 揃っているブロックの存在確認用配列初期化
	for (int z = 0; z < mCurrentStageData.mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData.mMapSizeWidth; x++)
		{
			mCheckboolMap[z][x] = false;
		}
	}

	// 探索基準のマスをチェック済みにする
	mCheckMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = -1;
	mCheckboolMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = true;

	mDiceAlignCnt = 1;
	CheckDiceAlign(_dice->GetMapPos(), _dice->GetTopDiceType());

	// 出目の数以上なければ返す
	if (mDiceAlignCnt < _dice->GetTopDiceTypeNum())
		return;

	// 対象のサイコロの下げる関数を呼ぶ
	for (int z = 0; z < mCurrentStageData.mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData.mMapSizeWidth; x++)
		{
			if (mCheckboolMap[z][x] && mDiceMap[z][x] != -1)
			{
				// Diceを落とす

				mDiceList[mDiceMap[z][x]]->SetDownPosition();

			}
		}
	}
}

void DiceManager::SetRemoveDice(Dice* _dice)
{
	auto itDice = std::find(mDiceList.begin(), mDiceList.end(), _dice);
	mDiceMap[(*itDice)->GetMapPos().z][(*itDice)->GetMapPos().x] = -1;
	/// Todo:参照している添え字が合わなくなるので消せない。修正方法考える
	//mDiceList.erase(itDice);
}

void DiceManager::CheckDiceAlign(INT3 _mapPos, DICEFRUIT _diceType)
{
	INT3 ans;
	// 上確認
	if (_mapPos.z > 0)
	{
		ans = { _mapPos.x ,0, _mapPos.z - 1 };
		// ブロックが存在してチェックされていないマスなら
		if (mCheckMap[ans.z][ans.x] > -1)
		{
			// チェック済みにする
			mCheckMap[ans.z][ans.x] = -1;
			// ブロックの面が同じか配列に入れる
			mCheckboolMap[ans.z][ans.x] = (mDiceList[mDiceMap[ans.z][ans.x]]->GetTopDiceType() == _diceType);
			// 面が同じだったら
			if (mCheckboolMap[ans.z][ans.x])
			{
				// サイコロのカウントを1足す
				mDiceAlignCnt++;
				// 揃っているブロックを基準にチェックする
				CheckDiceAlign(ans, _diceType);
			}
		}
	}
	// 下確認
	if (_mapPos.z < mCurrentStageData.mMapSizeHeight - 1)
	{
		ans = { _mapPos.x ,0, _mapPos.z + 1 };
		// ブロックが存在してチェックされていないマスなら
		if (mCheckMap[ans.z][ans.x] > -1)
		{
			// チェック済みにする
			mCheckMap[ans.z][ans.x] = -1;
			// ブロックの面が同じか配列に入れる
			mCheckboolMap[ans.z][ans.x] = (mDiceList[mDiceMap[ans.z][ans.x]]->GetTopDiceType() == _diceType);
			// 面が同じだったら
			if (mCheckboolMap[ans.z][ans.x])
			{
				// サイコロのカウントを1足す
				mDiceAlignCnt++;
				// 揃っているブロックを基準にチェックする
				CheckDiceAlign(ans, _diceType);
			}
		}
	}
	// 左確認
	if (_mapPos.x > 0)
	{
		ans = { _mapPos.x - 1 ,0, _mapPos.z };
		// ブロックが存在してチェックされていないマスなら
		if (mCheckMap[ans.z][ans.x] > -1)
		{
			// チェック済みにする
			mCheckMap[ans.z][ans.x] = -1;
			// ブロックの面が同じか配列に入れる
			mCheckboolMap[ans.z][ans.x] = (mDiceList[mDiceMap[ans.z][ans.x]]->GetTopDiceType() == _diceType);
			// 面が同じだったら
			if (mCheckboolMap[ans.z][ans.x])
			{
				// サイコロのカウントを1足す
				mDiceAlignCnt++;
				// 揃っているブロックを基準にチェックする
				CheckDiceAlign(ans, _diceType);
			}
		}
	}
	// 右確認
	if (_mapPos.x < mCurrentStageData.mMapSizeWidth - 1)
	{
		ans = { _mapPos.x + 1  ,0, _mapPos.z };
		// ブロックが存在してチェックされていないマスなら
		if (mCheckMap[ans.z][ans.x] > -1)
		{
			// チェック済みにする
			mCheckMap[ans.z][ans.x] = -1;
			// ブロックの面が同じか配列に入れる
			mCheckboolMap[ans.z][ans.x] = (mDiceList[mDiceMap[ans.z][ans.x]]->GetTopDiceType() == _diceType);
			// 面が同じだったら
			if (mCheckboolMap[ans.z][ans.x])
			{
				// サイコロのカウントを1足す
				mDiceAlignCnt++;
				// 揃っているブロックを基準にチェックする
				CheckDiceAlign(ans, _diceType);
			}
		}
	}
}