#include	"dice_manager.h"
#include	"stagedata_manager.h"
#include	"../component/map_pos_component.h"
#include	"../component/map_move_component.h"
#include	"../component/model_component.h"
#include	"../component/collision_component.h"
#include	"../../system/model/ModelMgr.h"
#include	<random>
#include	"../../system/util/XAudio2.h"
#include	"../../system/dx11/CDirectInput.h"

std::random_device rnd;							// 非決定的な乱数生成器
std::mt19937 mt(rnd());							// メルセンヌ・ツイスタの32ビット版、引数は初期シード値
std::uniform_int_distribution<> rand100(0, 99); // [0, 99] 範囲の一様乱数

using namespace DirectX;

void DiceManager::PuzzleInit()
{
	for (auto &obj : mpDiceList)
	{
		obj->GetComponent<Component::Collision>()->SetIsDraw(true);
	}
}

void DiceManager::DiceMapCreate(bool _isUp = true)
{
	mCurrentStageData = StageDataManager::GetInstance().GetCurrentStage();
	int diceCnt = 0;

	for (int z = 0; z < mCurrentStageData->mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData->mMapSizeWidth; x++)
		{
			mDiceMap[z][x] = NODICE;
		}
	}

	for (diceCnt = 0; diceCnt < mCurrentStageData->mDiceMtx.size(); diceCnt++)
	{
		if (diceCnt >= mpDiceList.size())
		{
			// Dice生成
			Dix::sp<Dice> dice;
			dice.SetPtr(new Dice);

			std::cout << "生成" << dice->GetObjectID() << "\n";

			SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(dice);
			mpDiceList.emplace_back(dice);	// vector配列に追加
		}

		mpDiceList[diceCnt]->GetTransform()->SetWordMtx(mCurrentStageData->mDiceMtx[diceCnt]);
		Float3 dicePos = mpDiceList[diceCnt]->GetTransform()->GetPosition();
		mpDiceList[diceCnt]->GetTransform()->SetPositionMove(dicePos);


		INT2 mapPos(static_cast<int>(dicePos.x / DICE_SCALE), static_cast<int>(-dicePos.z / DICE_SCALE));
		std::cout << "x" << mapPos.x << "z" << mapPos.z;

		mDiceMap[mapPos.z][mapPos.x] = mpDiceList[diceCnt]->GetObjectID();

		mpDiceList[diceCnt]->GetComponent<Component::Collision>()->SetColor(DirectX::XMFLOAT4(1, 1, 1, 0.0f));
		//mpDiceList[diceCnt]->GetComponent<Component::Collision>()->SetIsDraw(false);
		mpDiceList[diceCnt]->SetMapPos(INT3(mapPos.x, 0, mapPos.z));
		mpDiceList[diceCnt]->SetName(("Dice" + std::to_string(mDiceMap[mapPos.z][mapPos.x])));	// オブジェクトの名前に添え字を加える

		if (_isUp)
			mpDiceList[diceCnt]->GetTransform()->SetPositionY(-DICE_SCALE_HALF);
		else
			mpDiceList[diceCnt]->GetTransform()->SetPositionY(DICE_SCALE_HALF);

		mpDiceList[diceCnt]->Init();
		mpDiceList[diceCnt]->SetOverPlane();
	}

	for (int i = diceCnt; i < mpDiceList.size(); i++)
	{
		mpDiceList[i]->SetObjectState(ObjectState::eDead);
	}

	if (diceCnt < mpDiceList.size())
	{
		mpDiceList.erase(mpDiceList.begin() + diceCnt, mpDiceList.end());
		mpDiceList.shrink_to_fit();
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

void DiceManager::EndllesInit()
{
	Uninit();
	mFrameCnt = 0;
	mCurrentStageData = StageDataManager::GetInstance().GetCurrentStage();
	for (int z = 0; z < mCurrentStageData->mMapSizeHeight; z++)
		for (int x = 0; x < mCurrentStageData->mMapSizeWidth; x++)
			mDiceMap[z][x] = NODICE;
}

void DiceManager::EndleesUpdate()
{
	if (mEndlessCnt < 6)
	{
		mEndlessCnt++;

		// ランダムで出したマップ位置にDiceが存在すればコンティニュー
		int num = rand100(mt) % (mCurrentStageData->mMapSizeWidth*mCurrentStageData->mMapSizeHeight);
		int z = num / mCurrentStageData->mMapSizeWidth;
		int x = num % mCurrentStageData->mMapSizeHeight;
		if (mDiceMap[z][x] != NODICE)
			return;
		// プレイヤーとその周りで、他が埋まっていなければコンティニュー
		if (x <= mPlayerPos.x + 1 && x >= mPlayerPos.x - 1 && z <= mPlayerPos.z + 1 && z >= mPlayerPos.z - 1)
			if (mpDiceList.size() < mCurrentStageData->mMapSizeWidth*mCurrentStageData->mMapSizeHeight - 9)
				return;

		// Dice生成
		Dix::sp<Dice> dice;
		dice.SetPtr(new Dice);
		dice->GetTransform()->SetPositionMove(Float3(DICE_SCALE*x, -DICE_SCALE_HALF, -DICE_SCALE * z));
		dice->GetTransform()->SetAngle(mSpawnAngle[GetDiceRandomNum(rand100(mt))]);
		dice->GetTransform()->CreateWordMtx();

		// Y軸をランダムで回転
		XMFLOAT4X4 angleMtx;
		DX11MtxRotationY(mSpawnAngle[5 + rand100(mt) % 4].y, angleMtx);
		DX11MtxMultiply(dice->GetTransform()->worldMtx, angleMtx, dice->GetTransform()->worldMtx);

		dice->SetMapPos(INT3(x, 0, z));
		mDiceMap[z][x] = dice->GetObjectID();
		dice->SetName(("Dice" + std::to_string(mDiceMap[z][x])));	// オブジェクトの名前に添え字を加える
		dice->Init();

		mpDiceList.emplace_back(dice);	// vector配列に追加
		SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(dice);
	}

	/// Todo:埋まってたらゲームオーバー処理
	if (mCurrentStageData->mMapSizeWidth*mCurrentStageData->mMapSizeHeight <= mpDiceList.size())
	{
		return;
	}
	if (mFrameCnt < 200)
	{
		mFrameCnt++;
		return;
	}
	mFrameCnt = 0;
	// ランダム生成
	while (true)
	{
		// ランダムで出したマップ位置にDiceが存在すればコンティニュー
		int num = rand100(mt) % (mCurrentStageData->mMapSizeWidth*mCurrentStageData->mMapSizeHeight);
		int z = num / mCurrentStageData->mMapSizeWidth;
		int x = num % mCurrentStageData->mMapSizeHeight;
		if (mDiceMap[z][x] != NODICE)
			continue;
		// プレイヤーとその周りで、他が埋まっていなければコンティニュー
		if (x <= mPlayerPos.x + 1 && x >= mPlayerPos.x - 1 && z <= mPlayerPos.z + 1 && z >= mPlayerPos.z - 1)
			if (mpDiceList.size() < mCurrentStageData->mMapSizeWidth*mCurrentStageData->mMapSizeHeight - 9)
				continue;

		// Dice生成
		Dix::sp<Dice> dice;
		dice.SetPtr(new Dice);
		dice->GetTransform()->SetPositionMove(Float3(DICE_SCALE*x, -DICE_SCALE_HALF, -DICE_SCALE * z));
		dice->GetTransform()->SetAngle(mSpawnAngle[GetDiceRandomNum(rand100(mt))]);
		dice->GetTransform()->CreateWordMtx();

		// Y軸をランダムで回転
		XMFLOAT4X4 angleMtx;
		DX11MtxRotationY(mSpawnAngle[5 + rand100(mt) % 4].y, angleMtx);
		DX11MtxMultiply(dice->GetTransform()->worldMtx, angleMtx, dice->GetTransform()->worldMtx);

		dice->SetMapPos(INT3(x, 0, z));
		mDiceMap[z][x] = dice->GetObjectID();
		dice->SetName(("Dice" + std::to_string(mDiceMap[z][x])));	// オブジェクトの名前に添え字を加える
		dice->Init();

		mpDiceList.emplace_back(dice);	// vector配列に追加
		SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(dice);
		return;
	}
}

void DiceManager::ImguiDraw()
{
	ImGui::Text(std::to_string(mpDiceList.size()).c_str());

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
	for (int z = 0; z < STAGESIZEMAX; z++)
		for (int x = 0; x < STAGESIZEMAX; x++)
			mDiceMap[z][x] = NODICE;

	for (auto &obj : mpDiceList)
	{
		obj->SetObjectState(ObjectState::eDead);
	}
	mpDiceList.clear();
}

void DiceManager::SetPuzzle()
{
	for (auto &obj : mpDiceList)
	{
		obj->SetDiceSts(DiceStatus::eNormal);
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
	if (afterPos.z < 0 || afterPos.x < 0 || afterPos.z >= mCurrentStageData->mMapSizeHeight || afterPos.x >= mCurrentStageData->mMapSizeWidth)
		return false;
	// ステージ外なら移動しない
	if (mCurrentStageData->mFloorMap[afterPos.z][afterPos.x] <= 0)
		return false;

	// 行き先のサイコロのポインタを取得
	Dix::wp<Dice> afterDice = GetListInDice(afterPos.x, afterPos.z);
	// サイコロがあるとき
	if (afterDice.IsExist())
	{
		DiceStatus dSts = afterDice->GetDiceStatus();
		// 半分以上存在しているサイコロなら移動しない
		if (dSts == DiceStatus::eNormal || dSts == DiceStatus::eDown || dSts == DiceStatus::eUp)
			return false;
		// 半分以下のサイコロならそのサイコロを消す
		SetRemoveDice(afterDice->GetObjectID());
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
	if (afterPos.z < 0 || afterPos.x < 0 || afterPos.z >= mCurrentStageData->mMapSizeHeight || afterPos.x >= mCurrentStageData->mMapSizeWidth)
		return true;
	// ステージ外なら移動しない
	if (mCurrentStageData->mFloorMap[afterPos.z][afterPos.x] <= 0)
		return true;
	return false;
}

void DiceManager::CheckAligned(Dice* _dice)
{
	Dix::wp<Dice> ansDice;

	// ステップ数減らす
	if (mIsStepCount)
	{
		mStepCount--;
		if (mStepCount == 1)
		{
			PlaySound(SOUND_LABEL_SE_ONE);
		}
		else if (mStepCount == 2)
		{
			PlaySound(SOUND_LABEL_SE_TWO);
		}
		else if (mStepCount == 3)
		{
			PlaySound(SOUND_LABEL_SE_THREE);
		}
	}

	/// ハッピーワンチェック ////////////////////////////////////////////////
	if (_dice->GetTopDiceTypeNum() == 1)
	{
		INT3 mapPos = _dice->GetMapPos();
		INT3 ans[4] = { INT3(mapPos.x, 0, mapPos.z - 1), INT3(mapPos.x, 0, mapPos.z + 1), INT3(mapPos.x - 1, 0, mapPos.z), INT3(mapPos.x + 1, 0, mapPos.z) };
		for (int i = 0; i < 4; i++)
		{
			if ((i == 0 && ans[i].z < 0) || (i == 1 && ans[i].z > mCurrentStageData->mMapSizeHeight - 1))
				continue;
			if ((i == 2 && ans[i].x < 0) || (i == 3 && ans[i].x > mCurrentStageData->mMapSizeWidth - 1))
				continue;

			ansDice = GetListInDice(ans[i].x, ans[i].z);
			if (ansDice.IsExist())
			{
				if (ansDice->GetDiceStatus() == DiceStatus::eDown || ansDice->GetDiceStatus() == DiceStatus::eHalfDown)
				{
					for (auto d : mpDiceList)
					{
						if (d->GetObjectID() == _dice->GetObjectID())
							continue;
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
	for (int z = 0; z < mCurrentStageData->mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData->mMapSizeWidth; x++)
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

	// 対象サイコロの下げる関数を呼ぶ
	for (int z = 0; z < mCurrentStageData->mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData->mMapSizeWidth; x++)
		{
			if (mCheckboolMap[z][x] && mDiceMap[z][x] != NODICE)
			{
				// Diceを落とす
				GetListInDice(x, z)->SetDownPosition();
			}
		}
	}
}

void DiceManager::SetRemoveDice(int _diceId)
{
	//for (auto itDice : mpDiceList)
	for (auto itDice = mpDiceList.begin(); itDice != mpDiceList.end();)
	{
		if ((*itDice)->GetObjectID() == _diceId)
		{
			(*itDice)->SetObjectState(ObjectState::eDead);
			if (itDice != mpDiceList.end())
			{
				mDiceMap[(*itDice)->GetMapPos().z][(*itDice)->GetMapPos().x] = NODICE;
				mpDiceList.erase(itDice);
				mpDiceList.shrink_to_fit();
			}
			return;
		}
		itDice++;
	}
}

Dix::wp<Dice> DiceManager::GetDice(INT3 _mapPos)
{
	// ステージ外かDiceがなければnullptrを返す
	if (_mapPos.z < 0 || _mapPos.z >= mCurrentStageData->mMapSizeHeight)
		return NULL;
	if (_mapPos.x < 0 || _mapPos.x >= mCurrentStageData->mMapSizeWidth)
		return NULL;
	if (mDiceMap[_mapPos.z][_mapPos.x] == NODICE)
		return NULL;
	return GetListInDice(_mapPos.x, _mapPos.z);
}


void DiceManager::CheckDiceAlign(INT3 _mapPos, DiceFruit _diceType)
{
	INT3 ans[4] = { INT3(_mapPos.x, 0, _mapPos.z - 1), INT3(_mapPos.x, 0, _mapPos.z + 1), INT3(_mapPos.x - 1, 0, _mapPos.z), INT3(_mapPos.x + 1, 0, _mapPos.z) };
	for (int i = 0; i < 4; i++)
	{
		if ((i == 0 && ans[i].z < 0) || (i == 1 && ans[i].z > mCurrentStageData->mMapSizeHeight - 1))
			continue;
		if ((i == 2 && ans[i].x < 0) || (i == 3 && ans[i].x > mCurrentStageData->mMapSizeWidth - 1))
			continue;

		// ブロックが存在してチェックされていないマスなら
		if (mCheckMap[ans[i].z][ans[i].x] > NODICE)
		{
			// チェック済みにする
			mCheckMap[ans[i].z][ans[i].x] = NODICE;
			// ブロックの面が同じか配列に入れる
			mCheckboolMap[ans[i].z][ans[i].x] = (GetListInDice(ans[i].x, ans[i].z)->GetTopDiceType() == _diceType);
			// 面が同じだったら
			if (mCheckboolMap[ans[i].z][ans[i].x])
			{
				// サイコロのカウントを1足す
				mDiceAlignCnt++;
				// 揃っているブロックを基準にチェックする
				CheckDiceAlign(ans[i], _diceType);
			}
		}
	}
}

Dix::wp<Dice> DiceManager::GetListInDice(int x, int z)
{
	for (auto dice : mpDiceList)
	{
		if (dice->GetObjectID() == mDiceMap[z][x])
			return dice;
	}
	return NULL;
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

void DiceManager::CreateInit()
{
	mSelectNum = 0;
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/dice/Dice.fbx",
		"shader/vs.hlsl", "shader/toonps.hlsl",
		"assets/model/dice/");
	if (!sts)
	{
		MessageBox(nullptr, "Diceモデル 読み込みエラー", "error", MB_OK);
	}


}

void DiceManager::CreateUpdate()
{
	mCurrentStageData = StageDataManager::GetInstance().GetCurrentStage();

	if (mpDiceList.size() > 1)
	{
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_Q))
		{
			mSelectNum--;
			if (mSelectNum < 0)
				mSelectNum = static_cast<int>(mpDiceList.size()) - 1;
		}
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_E))
		{
			mSelectNum++;
			mSelectNum = mSelectNum % mpDiceList.size();
		}
	}

	// ステージ外にサイコロが存在した時の処理
	for (int i = 0; i < mpDiceList.size(); ++i)
	{
		if (mpDiceList[i]->GetComponent<Component::MapPos>()->GetMapPos().x > mCurrentStageData->mMapSizeWidth - 1 || mpDiceList[i]->GetComponent<Component::MapPos>()->GetMapPos().z > mCurrentStageData->mMapSizeHeight - 1)
		{
			mpDiceList[i]->SetObjectState(ObjectState::ePaused);

			mpDiceList[i]->GetComponent<Component::Collision>()->SetIsDraw(false);
			mpDiceList[i]->GetComponent<Component::Model>()->SetIsDraw(false);
		}
		else
		{
			mpDiceList[i]->SetObjectState(ObjectState::eActive);
			mpDiceList[i]->GetComponent<Component::Collision>()->SetIsDraw(true);
			mpDiceList[i]->GetComponent<Component::Model>()->SetIsDraw(true);
		}
	}

	if (mpDiceList.empty())
		return;

	if (!mIsSelect)
	{
		for (int i = 0; i < mpDiceList.size(); ++i)
		{
			mpDiceList[i]->GetComponent<Component::Collision>()->SetIsDraw(false);
			mpDiceList[i]->GetComponent<Component::MapMove>()->SetState(ObjectState::ePaused);
		}
		return;
	}

	for (int i = 0; i < mpDiceList.size(); ++i)
	{
		if (i == mSelectNum)
		{
			mpDiceList[i]->GetComponent<Component::Collision>()->SetIsDraw(true);
			mpDiceList[i]->GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 1, 1, 0.5f));
			mpDiceList[i]->GetComponent<Component::MapMove>()->SetState(ObjectState::eActive);
		}
		else
		{
			mpDiceList[i]->GetComponent<Component::Collision>()->SetIsDraw(false);
			mpDiceList[i]->GetComponent<Component::MapMove>()->SetState(ObjectState::ePaused);
		}
	}
}

void DiceManager::CreateImguiDraw()
{
	std::string str;
	ImGui::Text(u8"Q E：Dice切り替え");

	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 130), ImGuiWindowFlags_NoTitleBar);
	for (int i = 0; i < mpDiceList.size(); ++i)
	{
		str = "Dice " + std::to_string(i);
		ImGui::RadioButton(str.c_str(), &mSelectNum, i);
	}
	ImGui::EndChild();

	// サイコロ追加
	if (ImGui::Button("DiceAdd"))
		CreateAddDice();

	if (!mpDiceList.empty())
	{
		ImGui::SameLine();
		if (ImGui::Button("DiceRemove"))
		{
			// サイコロ削除
			SetCreateRemoveDice(mpDiceList[mSelectNum]->GetObjectID());
			if (mSelectNum > 0)
				mSelectNum--;
		}
	}

	if (mpDiceList.empty())
		return;

	if (mpDiceList[mSelectNum]->GetObjectState() != ObjectState::eActive)
		ImGui::Text(u8"ステージ外です");
	else
		mpDiceList[mSelectNum]->ImguiCreateDraw();
}


bool DiceManager::CreateAddDice()
{
	mCurrentStageData = StageDataManager::GetInstance().GetCurrentStage();
	for (int z = 0; z < mCurrentStageData->mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData->mMapSizeWidth; x++)
		{
			if (mDiceMap[z][x] < 1)
			{
				// Dice生成
				Dix::sp<Dice> dice;
				dice.SetPtr(new Dice);
				dice->SetDiceSts(DiceStatus::eCreate);
				dice->Init();
				dice->GetTransform()->SetPositionXYZ(Float3(DICE_SCALE*x, DICE_SCALE_HALF, -DICE_SCALE * z));
				dice->AddComponent<Component::MapPos>()->SetMapPos(INT2(x, z));
				dice->GetComponent<Component::MapPos>()->Init();
				dice->AddComponent<Component::MapMove>()->Init();
				dice->GetComponent<Component::Collision>()->SetInitState(ObjectTag::eDice, Float3(0, 0, 0), Float3(DICE_SCALE_HALF), DirectX::XMFLOAT4(1, 1, 1, 0.5f));
				dice->GetComponent<Component::Collision>()->Init();
				dice->GetComponent<Component::Collision>()->SetOrderInLayer(30);
				mDiceMap[z][x] = dice->GetObjectID();
				dice->SetName(("Dice" + std::to_string(mDiceMap[z][x])));	// オブジェクトの名前に添え字を加える

				mpDiceList.emplace_back(dice);	// vector配列に追加
				SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(dice);
				return true;
			}
		}
	}

	return false;
}

Dix::wp<Dice> DiceManager::GetCreateListInDice(int x, int z)
{
	for (auto dice : mpDiceList)
	{
		if (dice->GetObjectID() == mDiceMap[z][x])
			return dice;
	}
	return NULL;
}

INT2 DiceManager::GetMoveMapPos(Direction _direction, INT2 _mapPos)
{
	mCurrentStageData = StageDataManager::GetInstance().GetCurrentStage();

	INT2 ans = _mapPos;
	switch (_direction)
	{
	case Direction::eUp:
	{
		ans.z -= 1;
		while (true)
		{
			if (ans.z < 0)
				break;
			if (mDiceMap[ans.z][ans.x] < 1)
			{
				mDiceMap[ans.z][ans.x] = mDiceMap[_mapPos.z][_mapPos.x];
				mDiceMap[_mapPos.z][_mapPos.x] = NODICE;
				return ans - _mapPos;
			}
			ans.z -= 1;
		}
		break;
	}
	case Direction::eDown:
	{
		ans.z += 1;
		while (true)
		{
			if (ans.z > mCurrentStageData->mMapSizeHeight - 1)
				break;
			if (mDiceMap[ans.z][ans.x] < 1)
			{
				mDiceMap[ans.z][ans.x] = mDiceMap[_mapPos.z][_mapPos.x];
				mDiceMap[_mapPos.z][_mapPos.x] = NODICE;
				return ans - _mapPos;
			}
			ans.z += 1;
		}
		break;
	}
	case Direction::eLeft:
	{
		ans.x -= 1;
		while (true)
		{
			if (ans.x < 0)
				break;
			if (mDiceMap[ans.z][ans.x] < 1)
			{
				mDiceMap[ans.z][ans.x] = mDiceMap[_mapPos.z][_mapPos.x];
				mDiceMap[_mapPos.z][_mapPos.x] = NODICE;
				return ans - _mapPos;
			}
			ans.x -= 1;
		}
		break;
	}
	case Direction::eRight:
	{
		ans.x += 1;
		while (true)
		{
			if (ans.x > mCurrentStageData->mMapSizeWidth - 1)
				break;
			if (mDiceMap[ans.z][ans.x] < 1)
			{
				mDiceMap[ans.z][ans.x] = mDiceMap[_mapPos.z][_mapPos.x];
				mDiceMap[_mapPos.z][_mapPos.x] = NODICE;
				return ans - _mapPos;
			}
			ans.x += 1;
		}
		break;
	}
	}

	return INT2(0, 0);
}

void DiceManager::SetCreateRemoveDice(int _diceId)
{
	//for (auto itDice : mpDiceList)
	for (auto itDice = mpDiceList.begin(); itDice != mpDiceList.end();)
	{
		if ((*itDice)->GetObjectID() == _diceId)
		{
			(*itDice)->SetObjectState(ObjectState::eDead);
			if (itDice != mpDiceList.end())
			{
				mDiceMap[(*itDice)->GetComponent<Component::MapPos>()->GetMapPos().z][(*itDice)->GetComponent<Component::MapPos>()->GetMapPos().x] = NODICE;
				mpDiceList.erase(itDice);
				mpDiceList.shrink_to_fit();
			}
			return;
		}
		itDice++;
	}
}

Dix::wp<Dice> DiceManager::GetCreateDice(INT2 _mapPos)
{
	for (Dix::wp<Dice> dice : mpDiceList)
	{
		if (dice->GetComponent<Component::MapPos>()->GetMapPos() == _mapPos)
		{
			return dice;
		}
	}

	return NULL;
}

void DiceManager::SetCreateDiceMap()
{
	for (Dix::wp<Dice> dice : mpDiceList)
	{
		INT2 mapPos = dice->GetComponent<Component::MapPos>()->GetMapPos();
		mDiceMap[mapPos.z][mapPos.x] = dice->GetObjectID();
	}
}

void DiceManager::SetIsStepCount(bool _flg)
{
	mIsStepCount = _flg;
	mStepCount = mCurrentStageData->mStep;
}

bool DiceManager::GetIsAllAligned()
{
	for (Dix::wp<Dice> dice : mpDiceList)
	{
		// Diceが揃っている状態じゃなかったらfalseを返す
		if (dice->GetDiceStatus() != DiceStatus::eDown)
			return false;
	}
	return true;
}

void DiceManager::EndlessInit()
{
	Uninit();
	/// Todo:ランダムでいくつか生成
	mEndlessCnt = 0;
}

void DiceManager::DataCreate()
{
	Uninit();
	mCurrentStageData = StageDataManager::GetInstance().GetCurrentStage();
	for (int z = 0; z < mCurrentStageData->mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData->mMapSizeWidth; x++)
		{
			if (mCurrentStageData->mMap[z][x] < 0)
			{
				mDiceMap[z][x] = NODICE;
				continue;
			}

			// Dice生成
			Dix::sp<Dice> dice;
			dice.SetPtr(new Dice);
			dice->SetDiceSts(DiceStatus::eCreate);
			dice->Init();

			std::cout << "生成" << dice->GetObjectID() << "\n";

			//dice->GetTransform()->SetPositionXYZ(Float3(DICE_SCALE*x, DICE_SCALE_HALF, -DICE_SCALE * z));
			dice->AddComponent<Component::MapPos>()->Init();
			dice->GetComponent<Component::MapPos>()->SetMapPos(INT2(x, z));
			dice->AddComponent<Component::MapMove>()->Init();
			dice->GetComponent<Component::Collision>()->SetInitState(ObjectTag::eDice, Float3(0, 0, 0), Float3(DICE_SCALE_HALF), DirectX::XMFLOAT4(1, 1, 1, 0.5f));
			dice->GetComponent<Component::Collision>()->Init();
			dice->GetComponent<Component::Collision>()->SetOrderInLayer(30);
			mDiceMap[z][x] = dice->GetObjectID();
			dice->SetName(("Dice" + std::to_string(mDiceMap[z][x])));	// オブジェクトの名前に添え字を加える

			dice->GetTransform()->SetWordMtx(mCurrentStageData->mDiceMtx[mpDiceList.size()]);
			mpDiceList.emplace_back(dice);	// vector配列に追加
			SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(dice);

		}
	}
	for (int i = 0; i < mpDiceList.size(); ++i)
	{
		mpDiceList[i]->GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 1, 1, 0.5f));
		mpDiceList[i]->GetComponent<Component::Collision>()->SetIsDraw(false);
		mpDiceList[i]->GetComponent<Component::MapMove>()->SetState(ObjectState::ePaused);
	}
}