#include	"player_state_move.h"
#include	"../manager/dice_manager.h"
#include	"../manager/input_manager.h"
#include	"../../system/dx11/dx11_settransform.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_MOVE_MODEL		(0.085f)		// �ړ����x
#define	RATE_ROTATE_MODEL		(0.10f)			// ��]�����W��
#define	RATE_MOVE_MODEL			(0.10f)			// �ړ������W��
#define	GETOFFDICE_FRAME		(20)			// Dice����~���̂ɕK�v�ȃt���[����

using namespace PlayerState;

bool Move::CheckRoll()
{
	if (!mpOperationDice.IsExist() || *mpFoot != Foot::eDice)
		return false;
	if (mpOperationDice->GetDiceStatus() != DiceStatus::eNormal)
		return false;

	Float3 basePoint;	// ��_
	basePoint.x = static_cast<float>(mMapPos.x) * DICE_SCALE + DICE_SCALE_HALF;
	basePoint.z = -static_cast<float>(mMapPos.z) * DICE_SCALE - DICE_SCALE_HALF;

	// Dice�̊O���Ɍ������Ă��Ȃ���ΕԂ�
	switch (*mpDirection)
	{
	case Direction::eNeutral:
		return false;
	case Direction::eUp:
		if (basePoint.z + DICE_SCALE - mpTransform->scale.z / 2.0f > mpTransform->position.z)
			return false;
		break;
	case Direction::eDown:
		if (basePoint.z + mpTransform->scale.z / 2.0f < mpTransform->position.z)
			return false;
		break;
	case Direction::eLeft:
		if (basePoint.x - DICE_SCALE + mpTransform->scale.x / 2.0f < mpTransform->position.x)
			return false;
		break;
	case Direction::eRight:
		if (basePoint.x - mpTransform->scale.x / 2.0f > mpTransform->position.x)
			return false;
		break;
	}

	// ��]�\�ʒu�ɂ�����
	if (mpOperationDice->SetRollAction(*mpDirection))
	{
		mpTransform->move = 0;
		mpHolder->ChangeState(eRoll);	// ��Ԃ�ς���
		mpTransform->SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE + mpTransform->scale.y);
		return true;
	}
	// ��]�o���Ȃ����
	else
	{
		if (!mpOperationDice->CheckDiceDirection(*mpDirection))
			return false;
		switch (*mpDirection)
		{
		case Direction::eUp:
			mpTransform->SetPositionZ(basePoint.z + DICE_SCALE - mpTransform->scale.z / 2.0f);
			break;
		case Direction::eDown:
			mpTransform->SetPositionZ(basePoint.z + mpTransform->scale.z / 2.0f);
			break;
		case Direction::eLeft:
			mpTransform->SetPositionX(basePoint.x - DICE_SCALE + mpTransform->scale.x / 2.0f);
			break;
		case Direction::eRight:
			mpTransform->SetPositionX(basePoint.x - mpTransform->scale.x / 2.0f);
			break;
		}
		mpTransform->move = 0;
	}
	return false;
}

bool Move::CheckPush()
{
	if (*mpFoot != Foot::eFloor)
		return false;

	INT3 checkMapPos;

	// �ړ���������̃}�b�v�Ɉړ��\��
	switch (*mpDirection)
	{
	case Direction::eNeutral:
		return false;
	case Direction::eUp:
		if (-mMapPos.z*DICE_SCALE + DICE_SCALE_HALF - mpTransform->scale.z / 2.0f > mpTransform->position.z)
			return false;
		checkMapPos = INT3(mMapPos.x, 0, mMapPos.z - 1);
		break;
	case Direction::eDown:
		if (-mMapPos.z*DICE_SCALE - DICE_SCALE_HALF + mpTransform->scale.z / 2.0f < mpTransform->position.z)
			return false;
		checkMapPos = INT3(mMapPos.x, 0, mMapPos.z + 1);
		break;
	case Direction::eLeft:
		if (mMapPos.x*DICE_SCALE - DICE_SCALE_HALF + mpTransform->scale.x / 2.0f < mpTransform->position.x)
			return false;
		checkMapPos = INT3(mMapPos.x - 1, 0, mMapPos.z);
		break;
	case Direction::eRight:
		if (mMapPos.x*DICE_SCALE + DICE_SCALE_HALF - mpTransform->scale.x / 2.0f > mpTransform->position.x)
			return false;
		checkMapPos = INT3(mMapPos.x + 1, 0, mMapPos.z);
		break;
	}

	// �ړ�������Dice�����邩
	Dix::wp<Dice> checkDice = DiceManager::GetInstance()->GetDice(checkMapPos);

	if (!checkDice.IsExist())
		return false;

	switch (checkDice->GetDiceStatus())
	{
	case DiceStatus::eNormal:	// Dice�̏�Ԃ��ʏ�Ȃ牟��
		mpOperationDice = checkDice;
		if (mpOperationDice->SetPushAction(*mpDirection))
		{
			mpOperationDice = checkDice;
			mpHolder->ChangeState(ePush);	// ��Ԃ�ς���
			return true;
		}
		break;
	case DiceStatus::eUp:
		return false;
	case DiceStatus::eHalfUp:
		return false;
	case DiceStatus::eHalfDown:
		return false;
	case DiceStatus::eDown:
		return false;
	}

	// �ړ�����
	switch (*mpDirection)
	{
	case Direction::eUp:
		mpTransform->SetPositionZ(-mMapPos.z*DICE_SCALE + DICE_SCALE_HALF - mpTransform->scale.z / 2.0f);
		break;
	case Direction::eDown:
		mpTransform->SetPositionZ(-mMapPos.z*DICE_SCALE - DICE_SCALE_HALF + mpTransform->scale.z / 2.0f);
		break;
	case Direction::eLeft:
		mpTransform->SetPositionX(mMapPos.x*DICE_SCALE - DICE_SCALE_HALF + mpTransform->scale.x / 2.0f);
		break;
	case Direction::eRight:
		mpTransform->SetPositionX(mMapPos.x*DICE_SCALE + DICE_SCALE_HALF - mpTransform->scale.x / 2.0f);
		break;
	}
	mpTransform->move = 0;	// �ړ��ʂ�0�ɂ���
	return false;
}

void Move::SetMapPos()
{
	mMapPos.x = static_cast<int>((mpTransform->position.x + DICE_SCALE_HALF) / DICE_SCALE);
	mMapPos.z = static_cast<int>((mpTransform->position.z - DICE_SCALE_HALF) / DICE_SCALE) * -1;
	if (mMapPos.x < 0)	mMapPos.x = 0;
	if (mMapPos.z < 0)	mMapPos.z = 0;
}

void Move::MoveDirectionLimitDice()
{
	// �E
	if ((*mpDirection) != Direction::eRight && (*mpDirection) != Direction::eLeft)
		if (mMapPos.x*DICE_SCALE + DICE_SCALE_HALF - mpTransform->scale.x / 2.0f < mpTransform->GetPosition().x)
		{
			mpTransform->SetPositionX(mMapPos.x*DICE_SCALE + DICE_SCALE_HALF - mpTransform->scale.x / 2.0f);
			mpTransform->move.x = 0;
		}
	// ��
	if ((*mpDirection) != Direction::eRight && (*mpDirection) != Direction::eLeft)
		if (mMapPos.x*DICE_SCALE - DICE_SCALE_HALF + mpTransform->scale.x / 2.0f > mpTransform->GetPosition().x)
		{
			mpTransform->SetPositionX(mMapPos.x*DICE_SCALE - DICE_SCALE_HALF + mpTransform->scale.x / 2.0f);
			mpTransform->move.x = 0;
		}
	// ��
	if ((*mpDirection) != Direction::eUp && (*mpDirection) != Direction::eDown)
		if (-mMapPos.z*DICE_SCALE + DICE_SCALE_HALF - mpTransform->scale.z / 2.0f < mpTransform->GetPosition().z)
		{
			mpTransform->SetPositionZ(-mMapPos.z*DICE_SCALE + DICE_SCALE_HALF - mpTransform->scale.z / 2.0f);
			mpTransform->move.z = 0;
		}
	// ��
	if ((*mpDirection) != Direction::eUp && (*mpDirection) != Direction::eDown)
		if (-mMapPos.z*DICE_SCALE - DICE_SCALE_HALF + mpTransform->scale.z / 2.0f > mpTransform->GetPosition().z)
		{
			mpTransform->SetPositionZ(-mMapPos.z*DICE_SCALE - DICE_SCALE_HALF + mpTransform->scale.z / 2.0f);
			mpTransform->move.z = 0;
		}
}

void Move::MoveLimitDice(INT3 _dicePos)
{
	// �E
	if (_dicePos.x*DICE_SCALE + DICE_SCALE_HALF - mpTransform->scale.x / 2.0f < mpTransform->GetPosition().x)
	{
		mpTransform->SetPositionX(_dicePos.x*DICE_SCALE + DICE_SCALE_HALF - mpTransform->scale.x / 2.0f);
		if (DiceManager::GetInstance()->GetDice(INT3(_dicePos.x + 1, _dicePos.y, _dicePos.z)) != NULL)
		{
			mDiceDownFrame += GETOFFDICE_FRAME;
		}
		mDiceDownFrame++;
	}
	// ��
	if (_dicePos.x*DICE_SCALE - DICE_SCALE_HALF + mpTransform->scale.x / 2.0f > mpTransform->GetPosition().x)
	{
		mpTransform->SetPositionX(_dicePos.x*DICE_SCALE - DICE_SCALE_HALF + mpTransform->scale.x / 2.0f);
		if (DiceManager::GetInstance()->GetDice(INT3(_dicePos.x - 1, _dicePos.y, _dicePos.z)) != NULL)
		{
			mDiceDownFrame += GETOFFDICE_FRAME;
		}
		mDiceDownFrame++;
	}
	// ��
	if (-_dicePos.z*DICE_SCALE + DICE_SCALE_HALF - mpTransform->scale.z / 2.0f < mpTransform->GetPosition().z)
	{
		mpTransform->SetPositionZ(-_dicePos.z*DICE_SCALE + DICE_SCALE_HALF - mpTransform->scale.z / 2.0f);
		if (DiceManager::GetInstance()->GetDice(INT3(_dicePos.x, _dicePos.y, _dicePos.z - 1)) != NULL)
		{
			mDiceDownFrame += GETOFFDICE_FRAME;
		}
		mDiceDownFrame++;
	}
	// ��
	if (-_dicePos.z*DICE_SCALE - DICE_SCALE_HALF + mpTransform->scale.z / 2.0f > mpTransform->GetPosition().z)
	{
		mpTransform->SetPositionZ(-_dicePos.z*DICE_SCALE - DICE_SCALE_HALF + mpTransform->scale.z / 2.0f);
		if (DiceManager::GetInstance()->GetDice(INT3(_dicePos.x, _dicePos.y, _dicePos.z + 1)) != NULL)
		{
			mDiceDownFrame += GETOFFDICE_FRAME;
		}
		mDiceDownFrame++;
	}
}

void Move::Init()
{
	SetMapPos();
	mStageSize = mpHolder->GetStageSize();
	mDiceDownFrame = 0;
}

void Move::Exec()
{
	float radian;
	InputDirection inputDirection = InputManager::GetInstance().GetDirection(InputMode::eGame, static_cast<int>(GameAction::Move));
	if (inputDirection == InputDirection::eLeft)
	{
		// ���ړ�
		radian = XM_PI * 0.50f;
		mpTransform->move.x -= sinf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
		mpTransform->move.z -= cosf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = radian;
		(*mpDirection) = Direction::eLeft;
	}
	else if (inputDirection == InputDirection::eRight)
	{
		// �E�ړ�
		radian = -XM_PI * 0.50f;
		mpTransform->move.x -= sinf(radian) * VALUE_MOVE_MODEL;
		mpTransform->move.z -= cosf(radian) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = radian;
		(*mpDirection) = Direction::eRight;
	}
	else if (inputDirection == InputDirection::eUp)
	{
		// �O�ړ�
		mpTransform->move.x -= sinf(XM_PI) * VALUE_MOVE_MODEL;
		mpTransform->move.z -= cosf(XM_PI) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = XM_PI;
		(*mpDirection) = Direction::eUp;
	}
	else if (inputDirection == InputDirection::eDown)
	{
		// ��ړ�
		mpTransform->move.x -= sinf(0) * VALUE_MOVE_MODEL;
		mpTransform->move.z -= cosf(0) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = 0;
		(*mpDirection) = Direction::eDown;
	}
	else
	{
		// ���͂��Ă��Ȃ����
		mDiceDownFrame = 0;
	}

	// �ڕW�p�x�ƌ��݊p�x�Ƃ̍��������߂�
	float diffrot = mDestrot.y - mpTransform->rotation.y;
	if (diffrot > XM_PI)
	{
		diffrot -= XM_PI * 2.0f;
	}
	if (diffrot < -XM_PI)
	{
		diffrot += XM_PI * 2.0f;
	}

	// �p�x���x�Ɋ�����t����
	mpTransform->rotation.y += diffrot * RATE_ROTATE_MODEL;
	if (mpTransform->rotation.y > XM_PI)
	{
		mpTransform->rotation.y -= XM_PI * 2.0f;
	}
	if (mpTransform->rotation.y < -XM_PI)
	{
		mpTransform->rotation.y += XM_PI * 2.0f;
	}

	// �ʒu�ړ�
	mpTransform->AddPosition();
	// �ړ��ʂɊ�����������(��������)
	mpTransform->move += (mpTransform->move*-1.0f) * RATE_MOVE_MODEL;
	// ��]�𔽉f�A���s�ړ��𔽉f
	mpTransform->angle = ((mpTransform->rotation* 180.0f) / XM_PI);

	// �d�͉�
	mpTransform->PositionCorrectionY(-1);
	if (mpTransform->position.y < DICE_SCALE_HALF - 3)
	{
		mpTransform->position.y = DICE_SCALE_HALF - 3;
	}

	// �㉺���E�ǂ̏���
	// ��
	if (-mStageSize.z*DICE_SCALE + DICE_SCALE_HALF + mpTransform->scale.z / 2.0f > mpTransform->position.z)
	{
		mpTransform->SetPositionZ(-mStageSize.z*DICE_SCALE + DICE_SCALE_HALF + mpTransform->scale.z / 2.0f);
		mpTransform->move = 0;
	}
	// ��
	if (0 + DICE_SCALE_HALF - mpTransform->scale.z / 2.0f < mpTransform->position.z)
	{
		mpTransform->SetPositionZ(0 + DICE_SCALE_HALF - mpTransform->scale.z / 2.0f);
		mpTransform->move = 0;
	}
	// �E
	if (mStageSize.x*DICE_SCALE - DICE_SCALE_HALF - mpTransform->scale.x / 2.0f < mpTransform->position.x)
	{
		mpTransform->SetPositionX(mStageSize.x*DICE_SCALE - DICE_SCALE_HALF - mpTransform->scale.x / 2.0f);
		mpTransform->move = 0;
	}
	// ��
	if (0 - DICE_SCALE_HALF + mpTransform->scale.x / 2.0f > mpTransform->position.x)
	{
		mpTransform->SetPositionX(0 - DICE_SCALE_HALF + mpTransform->scale.x / 2.0f);
		mpTransform->move = 0;
	}

	mpTransform->CreateWordMtx();
	MoveDirectionLimitDice();

	// �����̃}�b�v�ʒu�ɂ���Dice�̃|�C���^�擾
	mpOperationDice = DiceManager::GetInstance()->GetDice(mMapPos);

	// Dice�̗L���ő����̏�ԕύX
	if (!mpOperationDice.IsExist())
	{
		*mpFoot = Foot::eFloor;
		CheckPush();
		SetMapPos();
		return;
	}

	if (mpOperationDice->GetTransform()->GetPosition().y < mpTransform->position.y)
		*mpFoot = Foot::eDice;

	if (*mpFoot == Foot::eFloor)
	{
		if (mpOperationDice->GetDiceStatus() != DiceStatus::eNormal)
		{
			mpTransform->SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + mpTransform->scale.y - 0.5f);
		}
		mDiceDownFrame = 0;
	}
	else if (*mpFoot == Foot::eDice)
	{
		if (CheckRoll())
			return;

		INT3 dicePos = mpOperationDice->GetMapPos();
		switch (mpOperationDice->GetDiceStatus())
		{
		case DiceStatus::eUp:
			if (mDiceDownFrame < GETOFFDICE_FRAME)
				MoveLimitDice(dicePos);
			break;
		case DiceStatus::eHalfUp:
			if (mDiceDownFrame < GETOFFDICE_FRAME)
				MoveLimitDice(dicePos);
			break;
		case DiceStatus::eRoll:
			return;
		case DiceStatus::eDown:
			if (mDiceDownFrame < GETOFFDICE_FRAME)
				MoveLimitDice(dicePos);
			break;
		case DiceStatus::eHalfDown:
			if (mDiceDownFrame < GETOFFDICE_FRAME)
				MoveLimitDice(dicePos);
			break;
		}
		mpTransform->SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + mpTransform->scale.y);
	}
	SetMapPos();
}

void Move::BeforeChange()
{
	SetMapPos();
}

void Move::AfterChange()
{
}
