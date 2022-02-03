#include	"dice_manager.h"
#include	"stagedata_manager.h"
#include	"../component/map_pos_component.h"
#include	"../component/map_move_component.h"
#include	"../component/model_component.h"
#include	"../component/collision_component.h"
#include	"../../system/model/ModelMgr.h"
#include	<random>

std::random_device rnd;							// �񌈒�I�ȗ���������
std::mt19937 mt(rnd());							// �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
std::uniform_int_distribution<> rand100(0, 99); // [0, 99] �͈͂̈�l����

using namespace DirectX;

void DiceManager::DiceMapCreate(bool _isUp = true)
{
	Uninit();
	mFrameCnt = 0;
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

			// Dice����
			Dix::sp<Dice> dice;
			dice.SetPtr(new Dice);
			dice->GetTransform()->SetWordMtx(mCurrentStageData->mDiceMtx[mpDiceList.size()]);
			std::cout << "����" << dice->GetObjectID() << "\n";

			if (_isUp)
				dice->GetTransform()->SetPositionXYZ(Float3(DICE_SCALE*x, -DICE_SCALE_HALF, -DICE_SCALE * z));
			else
				dice->GetTransform()->SetPositionXYZ(Float3(DICE_SCALE*x, DICE_SCALE_HALF, -DICE_SCALE * z));
			dice->SetMapPos(INT3(x, 0, z));
			mDiceMap[z][x] = dice->GetObjectID();
			dice->SetName(("Dice" + std::to_string(mDiceMap[z][x])));	// �I�u�W�F�N�g�̖��O�ɓY������������
			dice->Init();
			mpDiceList.emplace_back(dice);	// vector�z��ɒǉ�
			SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(dice);
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

void DiceManager::EndleesUpdate()
{
	/// Todo:���܂��Ă���Q�[���I�[�o�[����
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
	// �����_������
	while (true)
	{
		// �����_���ŏo�����}�b�v�ʒu��Dice�����݂���΃R���e�B�j���[
		int num = rand100(mt) % (mCurrentStageData->mMapSizeWidth*mCurrentStageData->mMapSizeHeight);
		int z = num / mCurrentStageData->mMapSizeWidth;
		int x = num % mCurrentStageData->mMapSizeHeight;
		if (mDiceMap[z][x] != NODICE)
			continue;
		// �v���C���[�Ƃ��̎���ŁA�������܂��Ă��Ȃ���΃R���e�B�j���[
		if (x <= mPlayerPos.x + 1 && x >= mPlayerPos.x - 1 && z <= mPlayerPos.z + 1 && z >= mPlayerPos.z - 1)
			if (mpDiceList.size() < mCurrentStageData->mMapSizeWidth*mCurrentStageData->mMapSizeHeight - 9)
				continue;

		// Dice����
		Dix::sp<Dice> dice;
		dice.SetPtr(new Dice);
		dice->GetTransform()->SetPositionMove(Float3(DICE_SCALE*x, -DICE_SCALE_HALF, -DICE_SCALE * z));
		dice->GetTransform()->SetAngle(mSpawnAngle[GetDiceRandomNum(rand100(mt))]);
		dice->GetTransform()->CreateWordMtx();

		// Y���������_���ŉ�]
		XMFLOAT4X4 angleMtx;
		DX11MtxRotationY(mSpawnAngle[5 + rand100(mt) % 4].y, angleMtx);
		DX11MtxMultiply(dice->GetTransform()->worldMtx, angleMtx, dice->GetTransform()->worldMtx);

		dice->SetMapPos(INT3(x, 0, z));
		mDiceMap[z][x] = dice->GetObjectID();
		dice->SetName(("Dice" + std::to_string(mDiceMap[z][x])));	// �I�u�W�F�N�g�̖��O�ɓY������������
		dice->Init();

		mpDiceList.emplace_back(dice);	// vector�z��ɒǉ�
		SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(dice);
		return;
	}
}

void DiceManager::ImguiDraw()
{
	ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(280, 200), ImGuiCond_Once);
	ImGui::Begin(u8"DiceManager");
	ImGui::Text(u8"�����m��");
	for (int i = 0; i < 6; i++)
	{
		ImGui::DragInt(std::to_string(i + 1).c_str(), &mSpawnRate[i], 0.5f, 0, 99);
	}
	ImGui::End();
}

void DiceManager::Uninit()
{
	for (auto &obj : mpDiceList)
	{
		obj->SetObjectState(ObjectState::eDead);
	}
	mpDiceList.clear();
	mpDiceList.shrink_to_fit();
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
	/// Todo:������INT3��A�g�����鉽�������

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

	// �s���悪���Ȃ�ړ����Ȃ�
	if (afterPos.z < 0 || afterPos.x < 0 || afterPos.z >= mCurrentStageData->mMapSizeHeight || afterPos.x >= mCurrentStageData->mMapSizeWidth)
		return false;
	// �X�e�[�W�O�Ȃ�ړ����Ȃ�
	if (mCurrentStageData->mFloorMap[afterPos.z][afterPos.x] <= 0)
		return false;

	// �s����̃T�C�R���̃|�C���^���擾
	Dix::wp<Dice> afterDice = GetListInDice(afterPos.x, afterPos.z);
	// �T�C�R��������Ƃ�
	if (afterDice.IsExist())
	{
		DiceStatus dSts = afterDice->GetDiceStatus();
		// �����ȏ㑶�݂��Ă���T�C�R���Ȃ�ړ����Ȃ�
		if (dSts == DiceStatus::eNormal || dSts == DiceStatus::eDown || dSts == DiceStatus::eUp)
			return false;
		// �����ȉ��̃T�C�R���Ȃ炻�̃T�C�R��������
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

	// �s���悪���Ȃ�ړ����Ȃ�
	if (afterPos.z < 0 || afterPos.x < 0 || afterPos.z >= mCurrentStageData->mMapSizeHeight || afterPos.x >= mCurrentStageData->mMapSizeWidth)
		return true;
	// �X�e�[�W�O�Ȃ�ړ����Ȃ�
	if (mCurrentStageData->mFloorMap[afterPos.z][afterPos.x] <= 0)
		return true;
	return false;
}

void DiceManager::CheckAligned(Dice* _dice)
{
	Dix::wp<Dice> ansDice;

	/// �n�b�s�[�����`�F�b�N ////////////////////////////////////////////////
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
	/// �n�b�s�[�����`�F�b�N ////////////////////////////////////////////////

	// ���݂̃u���b�N�ʒu���`�F�b�N�p�z��ɃR�s�[
	memcpy((void *)mCheckMap, (void *)mDiceMap, sizeof(mDiceMap));

	// �����Ă���u���b�N�̑��݊m�F�p�z�񏉊���
	for (int z = 0; z < mCurrentStageData->mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData->mMapSizeWidth; x++)
		{
			mCheckboolMap[z][x] = false;
		}
	}

	// �T����̃}�X���`�F�b�N�ς݂ɂ���
	mCheckMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = NODICE;
	mCheckboolMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = true;

	mDiceAlignCnt = 1;
	CheckDiceAlign(_dice->GetMapPos(), _dice->GetTopDiceType());

	// �o�ڂ̐��ȏ�Ȃ���ΕԂ�
	if (mDiceAlignCnt < _dice->GetTopDiceTypeNum())
		return;

	// �Ώۂ̃T�C�R���̉�����֐����Ă�
	for (int z = 0; z < mCurrentStageData->mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData->mMapSizeWidth; x++)
		{
			if (mCheckboolMap[z][x] && mDiceMap[z][x] != NODICE)
			{
				// Dice�𗎂Ƃ�
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
	// �X�e�[�W�O��Dice���Ȃ����nullptr��Ԃ�
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

		// �u���b�N�����݂��ă`�F�b�N����Ă��Ȃ��}�X�Ȃ�
		if (mCheckMap[ans[i].z][ans[i].x] > NODICE)
		{
			// �`�F�b�N�ς݂ɂ���
			mCheckMap[ans[i].z][ans[i].x] = NODICE;
			// �u���b�N�̖ʂ��������z��ɓ����
			mCheckboolMap[ans[i].z][ans[i].x] = (GetListInDice(ans[i].x, ans[i].z)->GetTopDiceType() == _diceType);
			// �ʂ�������������
			if (mCheckboolMap[ans[i].z][ans[i].x])
			{
				// �T�C�R���̃J�E���g��1����
				mDiceAlignCnt++;
				// �����Ă���u���b�N����Ƀ`�F�b�N����
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
		MessageBox(nullptr, "Dice���f�� �ǂݍ��݃G���[", "error", MB_OK);
	}


}

void DiceManager::CreateUpdate()
{
	mCurrentStageData = StageDataManager::GetInstance().GetCurrentStage();

	// �X�e�[�W�O�ɃT�C�R�������݂������̏���
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
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 130), ImGuiWindowFlags_NoTitleBar);
	for (int i = 0; i < mpDiceList.size(); ++i)
	{
		str = "Dice " + std::to_string(i);
		ImGui::RadioButton(str.c_str(), &mSelectNum, i);
	}
	ImGui::EndChild();

	// �T�C�R���ǉ�
	if (ImGui::Button("DiceAdd"))
		CreateAddDice();

	if (!mpDiceList.empty())
	{
		ImGui::SameLine();
		if (ImGui::Button("DiceRemove"))
		{
			// �T�C�R���폜
			SetCreateRemoveDice(mpDiceList[mSelectNum]->GetObjectID());
			if (mSelectNum > 0)
				mSelectNum--;
		}
	}

	if (mpDiceList.empty())
		return;

	if (mpDiceList[mSelectNum]->GetObjectState() != ObjectState::eActive)
		ImGui::Text(u8"�X�e�[�W�O�ł�");
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
				bool sts = ModelMgr::GetInstance().LoadModel(
					"assets/model/dice/Dice.fbx",
					"shader/vs.hlsl", "shader/toonps.hlsl",
					"assets/model/dice/");
				if (!sts)
				{
					MessageBox(nullptr, "Dice���f�� �ǂݍ��݃G���[", "error", MB_OK);
				}
				// Dice����
				Dix::sp<Dice> dice;
				dice.SetPtr(new Dice);
				dice->SetDiceSts(DiceStatus::eCreate);
				dice->Init();
				dice->GetTransform()->SetPositionXYZ(Float3(DICE_SCALE*x, DICE_SCALE_HALF, -DICE_SCALE * z));
				dice->AddComponent<Component::MapPos>()->SetMapPos(INT2(x, z));
				dice->AddComponent<Component::MapMove>()->Init();
				dice->GetComponent<Component::Collision>()->SetInitState(ObjectTag::eDice, Float3(0, 0, 0), Float3(DICE_SCALE_HALF), DirectX::XMFLOAT4(1, 1, 1, 0.5f));
				dice->GetComponent<Component::Collision>()->Init();
				dice->GetComponent<Component::Collision>()->SetOrderInLayer(30);
				mDiceMap[z][x] = dice->GetObjectID();
				dice->SetName(("Dice" + std::to_string(mDiceMap[z][x])));	// �I�u�W�F�N�g�̖��O�ɓY������������

				mpDiceList.emplace_back(dice);	// vector�z��ɒǉ�
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
	if (mDiceMap[_mapPos.z][_mapPos.x] == NODICE)
		return NULL;
	return GetCreateListInDice(_mapPos.x, _mapPos.z);
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

			// Dice����
			Dix::sp<Dice> dice;
			dice.SetPtr(new Dice);
			dice->SetDiceSts(DiceStatus::eCreate);
			dice->Init();
			dice->GetTransform()->SetWordMtx(mCurrentStageData->mDiceMtx[mpDiceList.size()]);

			std::cout << "����" << dice->GetObjectID() << "\n";

			dice->GetTransform()->SetPositionXYZ(Float3(DICE_SCALE*x, DICE_SCALE_HALF, -DICE_SCALE * z));
			dice->AddComponent<Component::MapPos>()->SetMapPos(INT2(x, z));
			dice->AddComponent<Component::MapMove>()->Init();
			dice->GetComponent<Component::Collision>()->SetInitState(ObjectTag::eDice, Float3(0, 0, 0), Float3(DICE_SCALE_HALF), DirectX::XMFLOAT4(1, 1, 1, 0.5f));
			dice->GetComponent<Component::Collision>()->Init();
			dice->GetComponent<Component::Collision>()->SetOrderInLayer(30);
			mDiceMap[z][x] = dice->GetObjectID();
			dice->SetName(("Dice" + std::to_string(mDiceMap[z][x])));	// �I�u�W�F�N�g�̖��O�ɓY������������

			mpDiceList.emplace_back(dice);	// vector�z��ɒǉ�
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