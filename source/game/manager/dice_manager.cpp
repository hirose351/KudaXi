#include	"dice_manager.h"
#include	"stagedata_manager.h"
#include	<random>

#define		NODICE	(-1)

std::random_device rnd;							// 非決定的な乱数生成器
std::mt19937 mt(rnd());							// メルセンヌ・ツイスタの32ビット版、引数は初期シード値
std::uniform_int_distribution<> rand100(0, 99); // [0, 99] 範囲の一様乱数

void DiceManager::DiceMapCreate()
{
	Uninit();
	mCurrentStageData.SetStageData(StageDataManager::GetInstance().GetCurrentStage());
	for (int z = 0; z < mCurrentStageData.mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData.mMapSizeWidth; x++)
		{
			//if (mCurrentStageData.mMap[z][x] < 0)
			//{
			mDiceMap[z][x] = NODICE;
			//	continue;
			//}
			//Dice* dice = new Dice;
			//dice->GetTransform()->SetWordMtx(mCurrentStageData.mDiceMtx[mDiceList.size()]);

			//// 生成配列に入れる
			//mDiceMap[z][x] = dice->GetObjectID();

			///// todo:前プロジェクトの7培スケールに合わせる為に1/7
			//dice->GetTransform()->SetScale(1.0f / 7.0f);
			//dice->GetTransform()->CreateScaleMtx();

			//dice->GetTransform()->SetPositionMove(Float3(DICE_SCALE*x, -DICE_SCALE_HALF, -DICE_SCALE * z));
			////dice->GetTransform()->CreateMtx();
			//dice->SetMapPos(INT3(x, 0, z));
			//dice->SetName(("Dice" + std::to_string(mDiceMap[z][x])));
			//// vector配列に追加
			//mDiceList.emplace_back(dice);
		}
	}
}

DiceManager::DiceManager()
{
	mSpawnAngle[0] = { 0, 0, 0 };
	mSpawnAngle[1] = { 90.0f,0,0 };
	mSpawnAngle[2] = { 0,0,90.0f };
	mSpawnAngle[3] = { 0,0,-90.0f };
	mSpawnAngle[4] = { -90.0f,0,0 };
	mSpawnAngle[5] = { 180.0f,0,0 };
	mSpawnAngle[6] = { 0,90.0f,0 };
	mSpawnAngle[7] = { 0,180.0f,0 };
	mSpawnAngle[8] = { 0,-90.0f,0 };

}

void DiceManager::Init()
{
	DiceMapCreate();
}

void DiceManager::Update()
{
	if (mCurrentStageData.mMapSizeWidth*mCurrentStageData.mMapSizeHeight <= mDiceList.size())
		return;
	// ランダム生成
	static int cnt = 0;
	cnt++;

	if (cnt == 200)
	{
		cnt = 0;
		while (true)
		{
			// ランダムで出したマップ位置にDiceが存在すればコンティニュー
			int num = rand100(mt) % (mCurrentStageData.mMapSizeWidth*mCurrentStageData.mMapSizeHeight);
			int z = num / mCurrentStageData.mMapSizeWidth;
			int x = num % mCurrentStageData.mMapSizeHeight;
			if (mDiceMap[z][x] != NODICE)
				continue;
			// プレイヤーとその周りで、他が埋まっていなければコンティニュー
			if (x <= mPlayerPos.x + 1 && x >= mPlayerPos.x - 1 && z <= mPlayerPos.z + 1 && z >= mPlayerPos.z - 1)
				if (mDiceList.size() < mCurrentStageData.mMapSizeWidth*mCurrentStageData.mMapSizeHeight - 9)
					continue;

			// Dice生成
			Dice* dice = new Dice;
			dice->GetTransform()->SetPositionMove(Float3(DICE_SCALE*x, -DICE_SCALE_HALF, -DICE_SCALE * z));
			dice->GetTransform()->SetAngle(mSpawnAngle[GetDiceRandomNum(rand100(mt))]);
			dice->GetTransform()->CreateMtx();

			// Y軸をランダムで回転
			XMFLOAT4X4 angleMtx;
			DX11MtxRotationY(mSpawnAngle[5 + rand100(mt) % 4].y, angleMtx);
			DX11MtxMultiply(dice->GetTransform()->worldMtx, angleMtx, dice->GetTransform()->worldMtx);

			dice->SetMapPos(INT3(x, 0, z));
			mDiceMap[z][x] = dice->GetObjectID();
			dice->SetName(("Dice" + std::to_string(mDiceMap[z][x])));	// オブジェクトの名前に添え字を加える
			dice->Init();

			mDiceList.emplace_back(dice);	// vector配列に追加
			return;
		}
	}

}

void DiceManager::ImguiDraw()
{
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(280, 200), ImGuiCond_Once);
	ImGui::Begin(u8"DiceManager");
	ImGui::Text(u8"生成確率");
	for (int i = 0; i < 6; i++)
	{
		ImGui::DragInt(std::to_string(i + 1).c_str(), &mSpawnRate[i], 0.5f, 0, 99);
	}
	ImGui::End();
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

	// 行き先のサイコロのポインタを取得
	Dice* afterDice = GetListInDice(afterPos.x, afterPos.z);
	// サイコロがあるとき
	if (afterDice != nullptr)
	{
		DiceStatus dSts = afterDice->GetDiceStatus();
		// 半分以上存在しているサイコロなら移動しない
		if (dSts == DiceStatus::eNormal || dSts == DiceStatus::eDown || dSts == DiceStatus::eUp)
			return false;
		// 半分以下のサイコロならそのサイコロを消す
		SetRemoveDice(afterDice);
	}

	mDiceMap[afterPos.z][afterPos.x] = mDiceMap[_dice->GetMapPos().z][_dice->GetMapPos().x];
	mDiceMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = NODICE;
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
	Dice* ansDice;

	/// ハッピーワンチェック ////////////////////////////////////////////////
	if (_dice->GetTopDiceTypeNum() == 1)
	{
		INT3 ans;
		INT3 mapPos = _dice->GetMapPos();
		if (mapPos.z > 0)
		{
			ans = { mapPos.x ,0, mapPos.z - 1 };
			ansDice = GetListInDice(ans.x, ans.z);
			if (ansDice != nullptr)
			{
				if (ansDice->GetDiceStatus() == DiceStatus::eDown || ansDice->GetDiceStatus() == DiceStatus::eHalfDown)
				{
					for (auto d : mDiceList)
					{
						d->SetHappyOne();
					}
					return;
				}
			}
		}
		// 下確認
		if (mapPos.z < mCurrentStageData.mMapSizeHeight - 1)
		{
			ans = { mapPos.x ,0, mapPos.z + 1 };
			ansDice = GetListInDice(ans.x, ans.z);
			if (ansDice != nullptr)
			{
				if (ansDice->GetDiceStatus() == DiceStatus::eDown || ansDice->GetDiceStatus() == DiceStatus::eHalfDown)
				{
					for (auto d : mDiceList)
					{
						d->SetHappyOne();
					}
					return;
				}
			}
		}
		// 左確認
		if (mapPos.x > 0)
		{
			ans = { mapPos.x - 1 ,0, mapPos.z };
			ansDice = GetListInDice(ans.x, ans.z);
			if (ansDice != nullptr)
			{
				if (ansDice->GetDiceStatus() == DiceStatus::eDown || ansDice->GetDiceStatus() == DiceStatus::eHalfDown)
				{
					for (auto d : mDiceList)
					{
						d->SetHappyOne();
					}
					return;
				}
			}
		}
		// 右確認
		if (mapPos.x < mCurrentStageData.mMapSizeWidth - 1)
		{
			ans = { mapPos.x + 1  ,0, mapPos.z };
			ansDice = GetListInDice(ans.x, ans.z);
			if (ansDice != nullptr)
			{
				if (ansDice->GetDiceStatus() == DiceStatus::eDown || ansDice->GetDiceStatus() == DiceStatus::eHalfDown)
				{
					for (auto d : mDiceList)
					{
						d->SetHappyOne();
					}
					return;
				}
			}
		}
		return;
	}
	/// ハッピーワンチェック ////////////////////////////////////////////////

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
	mCheckMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = NODICE;
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
			if (mCheckboolMap[z][x] && mDiceMap[z][x] != NODICE)
			{
				// Diceを落とす
				GetListInDice(x, z)->SetDownPosition();
			}
		}
	}

	/// ハッピーワンチェック ////////////////////////////////////////////////

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
	mCheckMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = NODICE;
	mCheckboolMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = true;

	mDiceAlignCnt = 1;
	CheckDiceAlign(_dice->GetMapPos(), DiceFruit::eApple);
	if (mDiceAlignCnt < 2)
		return;
	for (auto d : mDiceList)
	{
		d->SetHappyOne();
	}
	/// ハッピーワンチェック ////////////////////////////////////////////////
}

void DiceManager::SetRemoveDice(Dice* _dice)
{
	auto itDice = std::find(mDiceList.begin(), mDiceList.end(), _dice);
	_dice->SetObjectState(ObjectState::eDead);
	if (itDice != mDiceList.end())
	{
		mDiceList.erase(itDice);
	}

	mDiceList.shrink_to_fit();
	mDiceMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = NODICE;
}

Dice* DiceManager::GetDice(INT3 _mapPos)
{
	// ステージ外かDiceがなければnullptrを返す
	if (_mapPos.z < 0 || _mapPos.z >= mCurrentStageData.mMapSizeHeight)
		return nullptr;
	if (_mapPos.x < 0 || _mapPos.x >= mCurrentStageData.mMapSizeWidth)
		return nullptr;
	if (mDiceMap[_mapPos.z][_mapPos.x] == NODICE)
		return nullptr;

	return GetListInDice(_mapPos.x, _mapPos.z);
}

void DiceManager::CheckDiceAlign(INT3 _mapPos, DiceFruit _diceType)
{
	INT3 ans[4] = { INT3(_mapPos.x, 0, _mapPos.z - 1), INT3(_mapPos.x, 0, _mapPos.z + 1), INT3(_mapPos.x - 1, 0, _mapPos.z), INT3(_mapPos.x + 1, 0, _mapPos.z) };
	for (auto it : ans)
	{
		if (it.z < 0 || it.z > mCurrentStageData.mMapSizeHeight - 1)
			return;
		if (it.x < 0 || it.x > mCurrentStageData.mMapSizeWidth - 1)
			return;
		// ブロックが存在してチェックされていないマスなら
		if (mCheckMap[it.z][it.x] > NODICE)
		{
			// チェック済みにする
			mCheckMap[it.z][it.x] = NODICE;
			// ブロックの面が同じか配列に入れる
			mCheckboolMap[it.z][it.x] = (GetListInDice(it.x, it.z)->GetTopDiceType() == _diceType);
			// 面が同じだったら
			if (mCheckboolMap[it.z][it.x])
			{
				// サイコロのカウントを1足す
				mDiceAlignCnt++;
				// 揃っているブロックを基準にチェックする
				CheckDiceAlign(it, _diceType);
			}
		}
	}
}

Dice* DiceManager::GetListInDice(int x, int z)
{
	for (auto dice : mDiceList)
	{
		if (dice->GetObjectID() == mDiceMap[z][x])
			return dice;
	}
	return nullptr;
}

int DiceManager::GetDiceRandomNum(int _rndNum)
{
	for (int i = 0; i < 6; i++)
	{
		if (_rndNum <= mSpawnRate[i])
			return i;
		_rndNum -= mSpawnRate[i];
		continue;
	}
	return 0;
}
