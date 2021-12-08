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
			// �����z��ɓ����
			mDiceMap[z][x] = static_cast<int>(mDiceList.size());
			Dice* dice = new Dice;
			dice->GetTransform()->ReSetValue();
			dice->GetTransform()->SetWordMtx(mCurrentStageData.mDiceMtx[mDiceList.size()]);


			dice->GetTransform()->SetScale(1.0f / 7.0f);
			dice->GetTransform()->CreateScaleMtx();

			dice->GetTransform()->SetPositionMove(Float3(DICE_SCALE*x, -DICE_SCALE / 2.0f, -DICE_SCALE * z));
			//dice->GetTransform()->CreateMtx();
			dice->SetMapPos(INT3(x, 0, z));
			// �I�u�W�F�N�g�̖��O�ɓY������������
			nameNum = ("Dice" + std::to_string(mDiceMap[z][x]));
			dice->SetName(nameNum);
			// vector�z��ɒǉ�
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
	if (afterPos.z < 0 || afterPos.x < 0 || afterPos.z >= mCurrentStageData.mMapSizeHeight || afterPos.x >= mCurrentStageData.mMapSizeWidth)
		return false;
	// �X�e�[�W�O�Ȃ�ړ����Ȃ�
	if (mCurrentStageData.mFloorMap[afterPos.z][afterPos.x] <= 0)
		return false;
	// �T�C�R��������Έړ����Ȃ�
	if (mDiceMap[afterPos.z][afterPos.x] > -1)
		return false;

	mDiceMap[afterPos.z][afterPos.x] = mDiceMap[_dice->GetMapPos().z][_dice->GetMapPos().x];
	mDiceMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = -1;
	_dice->SetMapPos(afterPos);
	return true;
}

void DiceManager::CheckAligned(Dice* _dice)
{
	// ���݂̃u���b�N�ʒu���`�F�b�N�p�z��ɃR�s�[
	memcpy((void *)mCheckMap, (void *)mDiceMap, sizeof(mDiceMap));


	// �����Ă���u���b�N�̑��݊m�F�p�z�񏉊���
	for (int z = 0; z < mCurrentStageData.mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData.mMapSizeWidth; x++)
		{
			mCheckboolMap[z][x] = false;
		}
	}

	// �T����̃}�X���`�F�b�N�ς݂ɂ���
	mCheckMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = -1;
	mCheckboolMap[_dice->GetMapPos().z][_dice->GetMapPos().x] = true;

	mDiceAlignCnt = 1;
	CheckDiceAlign(_dice->GetMapPos(), _dice->GetTopDiceType());

	// ������΃u���b�N��������֐����Ă�
	if (mDiceAlignCnt >= _dice->GetTopDiceTypeNum())
	{
		for (int z = 0; z < mCurrentStageData.mMapSizeHeight; z++)
		{
			for (int x = 0; x < mCurrentStageData.mMapSizeWidth; x++)
			{
				if (mCheckboolMap[z][x])
				{
					if (mDiceMap[z][x] != -1)
					{
						// Dice�𗎂Ƃ�
						mDiceList[mDiceMap[z][x]]->SetDownPosition();
					}
				}
			}
		}
	}
}

void DiceManager::CheckDiceAlign(INT3 _mapPos, DICEFRUIT _diceType)
{
	// ��m�F
	if (_mapPos.z > 0)
	{
		// �u���b�N�����݂��ă`�F�b�N����Ă��Ȃ��}�X�Ȃ�
		if (mCheckMap[_mapPos.z - 1][_mapPos.x] > -1)
		{
			// �`�F�b�N�ς݂ɂ���
			mCheckMap[_mapPos.z - 1][_mapPos.x] = -1;
			// �u���b�N�̖ʂ��������z��ɓ����
			mCheckboolMap[_mapPos.z - 1][_mapPos.x] = (mDiceList[mDiceMap[_mapPos.z - 1][_mapPos.x]]->GetTopDiceType() == _diceType);
			// �ʂ�������������
			if (mCheckboolMap[_mapPos.z - 1][_mapPos.x])
			{
				// �T�C�R���̃J�E���g��1����
				mDiceAlignCnt++;
				// �����Ă���u���b�N����Ƀ`�F�b�N����
				CheckDiceAlign(INT3(_mapPos.x, _mapPos.y, _mapPos.z - 1), _diceType);
			}
		}
	}
	// ���m�F
	if (_mapPos.z < mCurrentStageData.mMapSizeHeight - 1)
	{
		// �u���b�N�����݂��ă`�F�b�N����Ă��Ȃ��}�X�Ȃ�
		if (mCheckMap[_mapPos.z + 1][_mapPos.x] > -1)
		{
			// �`�F�b�N�ς݂ɂ���
			mCheckMap[_mapPos.z + 1][_mapPos.x] = -1;
			// �u���b�N�̖ʂ��������z��ɓ����
			mCheckboolMap[_mapPos.z + 1][_mapPos.x] = (mDiceList[mDiceMap[_mapPos.z + 1][_mapPos.x]]->GetTopDiceType() == _diceType);
			// �ʂ�������������
			if (mCheckboolMap[_mapPos.z + 1][_mapPos.x])
			{
				// �T�C�R���̃J�E���g��1����
				mDiceAlignCnt++;
				// �����Ă���u���b�N����Ƀ`�F�b�N����
				CheckDiceAlign(INT3(_mapPos.x, _mapPos.y, _mapPos.z + 1), _diceType);
			}
		}
	}
	// ���m�F
	if (_mapPos.x > 0)
	{
		// �u���b�N�����݂��ă`�F�b�N����Ă��Ȃ��}�X�Ȃ�
		if (mCheckMap[_mapPos.z][_mapPos.x - 1] > -1)
		{
			// �`�F�b�N�ς݂ɂ���
			mCheckMap[_mapPos.z][_mapPos.x - 1] = -1;
			// �u���b�N�̖ʂ��������z��ɓ����
			mCheckboolMap[_mapPos.z][_mapPos.x - 1] = (mDiceList[mDiceMap[_mapPos.z][_mapPos.x - 1]]->GetTopDiceType() == _diceType);
			// �ʂ�������������
			if (mCheckboolMap[_mapPos.z][_mapPos.x - 1])
			{
				// �T�C�R���̃J�E���g��1����
				mDiceAlignCnt++;
				// �����Ă���u���b�N����Ƀ`�F�b�N����
				CheckDiceAlign(INT3(_mapPos.x - 1, _mapPos.y, _mapPos.z), _diceType);
			}
		}
	}
	// �E�m�F
	if (_mapPos.x < mCurrentStageData.mMapSizeWidth - 1)
	{
		// �u���b�N�����݂��ă`�F�b�N����Ă��Ȃ��}�X�Ȃ�
		if (mCheckMap[_mapPos.z][_mapPos.x + 1] > -1)
		{
			// �`�F�b�N�ς݂ɂ���
			mCheckMap[_mapPos.z][_mapPos.x + 1] = -1;
			// �u���b�N�̖ʂ��������z��ɓ����
			mCheckboolMap[_mapPos.z][_mapPos.x + 1] = (mDiceList[mDiceMap[_mapPos.z][_mapPos.x + 1]]->GetTopDiceType() == _diceType);
			// �ʂ�������������
			if (mCheckboolMap[_mapPos.z][_mapPos.x + 1])
			{
				// �T�C�R���̃J�E���g��1����
				mDiceAlignCnt++;
				// �����Ă���u���b�N����Ƀ`�F�b�N����
				CheckDiceAlign(INT3(_mapPos.x + 1, _mapPos.y, _mapPos.z), _diceType);
			}
		}
	}
}