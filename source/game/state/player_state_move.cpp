#include	"player_state_move.h"
#include	"../manager/dice_manager.h"
#include	"../manager/input_manager.h"
#include	"../../system/dx11/DX11Settransform.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_MOVE_MODEL	(0.085f)				// �ړ����x
//#define	VALUE_MOVE_MODEL	(0.7f)				// �ړ����x
#define	VALUE_ROTATE_MODEL	(XM_PI * 0.025f)		// ��]���x
#define	RATE_ROTATE_MODEL	(0.10f)					// ��]�����W��
#define	RATE_MOVE_MODEL		(0.1f)

using namespace PlayerState;

bool Move::CheckRoll()
{
	if (!mpOperationDice.IsExist() || *mFoot != Foot::eDice)
		return false;
	if (mpOperationDice->GetDiceStatus() != DiceStatus::eNormal)
		return false;

	Float3 basePoint;// ��_
	basePoint.x = static_cast<float>(mMapPos.x) * DICE_SCALE + DICE_SCALE_HALF;
	basePoint.z = -static_cast<float>(mMapPos.z) * DICE_SCALE - DICE_SCALE_HALF;

	// Dice�̊O���Ɍ������Ă��Ȃ���ΕԂ�
	switch (*mDirection)
	{
	case Direction::eNeutral:
		return false;
	case Direction::eUp:
		if (basePoint.z + DICE_SCALE - mTransform->scale.z / 2.0f > mTransform->position.z)
			return false;
		break;
	case Direction::eDown:
		if (basePoint.z + mTransform->scale.z / 2.0f < mTransform->position.z)
			return false;
		break;
	case Direction::eLeft:
		if (basePoint.x - DICE_SCALE + mTransform->scale.x / 2.0f < mTransform->position.x)
			return false;
		break;
	case Direction::eRight:
		if (basePoint.x - mTransform->scale.x / 2.0f > mTransform->position.x)
			return false;
		break;
	}

	// ��]�\�ʒu�ɂ�����
	if (mpOperationDice->SetRollAction(*mDirection))
	{
		mTransform->move = 0;
		mHolder->ChangeState(eRoll);	// ��Ԃ�ς���
		mTransform->SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE + mTransform->scale.y);
		return true;
	}
	// ��]�o���Ȃ����
	else
	{
		if (!mpOperationDice->CheckDiceDirection(*mDirection))
			return false;
		switch (*mDirection)
		{
		case Direction::eUp:
			mTransform->SetPositionZ(basePoint.z + DICE_SCALE - mTransform->scale.z / 2.0f);
			break;
		case Direction::eDown:
			mTransform->SetPositionZ(basePoint.z + mTransform->scale.z / 2.0f);
			break;
		case Direction::eLeft:
			mTransform->SetPositionX(basePoint.x - DICE_SCALE + mTransform->scale.x / 2.0f);
			break;
		case Direction::eRight:
			mTransform->SetPositionX(basePoint.x - mTransform->scale.x / 2.0f);
			break;
		}
		mTransform->move = 0;
	}
	return false;
}

bool Move::CheckPush()
{
	if (*mFoot != Foot::eFloor)
		return false;

	INT3 checkMapPos;

	// �ړ���������̃}�b�v�Ɉړ��\��
	switch (*mDirection)
	{
	case Direction::eNeutral:
		return false;
	case Direction::eUp:
		if (-mMapPos.z*DICE_SCALE + DICE_SCALE_HALF - mTransform->scale.z / 2.0f > mTransform->position.z)
			return false;
		checkMapPos = INT3(mMapPos.x, 0, mMapPos.z - 1);
		break;
	case Direction::eDown:
		if (-mMapPos.z*DICE_SCALE - DICE_SCALE_HALF + mTransform->scale.z / 2.0f < mTransform->position.z)
			return false;
		checkMapPos = INT3(mMapPos.x, 0, mMapPos.z + 1);
		break;
	case Direction::eLeft:
		if (mMapPos.x*DICE_SCALE - DICE_SCALE_HALF + mTransform->scale.x / 2.0f < mTransform->position.x)
			return false;
		checkMapPos = INT3(mMapPos.x - 1, 0, mMapPos.z);
		break;
	case Direction::eRight:
		if (mMapPos.x*DICE_SCALE + DICE_SCALE_HALF - mTransform->scale.x / 2.0f > mTransform->position.x)
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
	case DiceStatus::eNormal:// Dice�̏�Ԃ��ʏ�Ȃ牟��
		mpOperationDice = checkDice;
		if (mpOperationDice->SetPushAction(*mDirection))
		{
			mpOperationDice = checkDice;
			mHolder->ChangeState(ePush);	// ��Ԃ�ς���
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

	mTransform->move = Float3(0, 0, 0);

	// �ړ�����
	switch (*mDirection)
	{
	case Direction::eUp:
		mTransform->SetPositionZ(-mMapPos.z*DICE_SCALE + DICE_SCALE_HALF - mTransform->scale.z / 2.0f);
		break;
	case Direction::eDown:
		mTransform->SetPositionZ(-mMapPos.z*DICE_SCALE - DICE_SCALE_HALF + mTransform->scale.z / 2.0f);
		break;
	case Direction::eLeft:
		mTransform->SetPositionX(mMapPos.x*DICE_SCALE - DICE_SCALE_HALF + mTransform->scale.x / 2.0f);
		break;
	case Direction::eRight:
		mTransform->SetPositionX(mMapPos.x*DICE_SCALE + DICE_SCALE_HALF - mTransform->scale.x / 2.0f);
		break;
	}
	return false;
}

void Move::SetMapPos()
{
	mMapPos.x = static_cast<int>((mTransform->position.x + DICE_SCALE_HALF) / DICE_SCALE);
	mMapPos.z = static_cast<int>((mTransform->position.z - DICE_SCALE_HALF) / DICE_SCALE) * -1;

	if (mMapPos.x < 0)
		mMapPos.x = 0;
	if (mMapPos.z < 0)
		mMapPos.z = 0;
}

void Move::Init()
{
	mStageSize = mHolder->GetStageSize();
}

void Move::Exec()
{
	Float3 rotCamera;
	float radian;
	InputDirection inputDirection = InputManager::GetInstance().GetDirection(InputMode::eGame, static_cast<int>(GameAction::Move));
	if (inputDirection == InputDirection::eLeft)
	{
		// ���ړ�
		radian = rotCamera.y + XM_PI * 0.50f;
		mTransform->move.x -= sinf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
		mTransform->move.z -= cosf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = radian;
		(*mDirection) = Direction::eLeft;
	}
	else if (inputDirection == InputDirection::eRight)
	{
		// �E�ړ�
		radian = rotCamera.y - XM_PI * 0.50f;
		mTransform->move.x -= sinf(radian) * VALUE_MOVE_MODEL;
		mTransform->move.z -= cosf(radian) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = radian;
		(*mDirection) = Direction::eRight;
	}
	else if (inputDirection == InputDirection::eUp)
	{
		// �O�ړ�
		mTransform->move.x -= sinf(XM_PI) * VALUE_MOVE_MODEL;
		mTransform->move.z -= cosf(XM_PI) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = rotCamera.y + XM_PI;
		(*mDirection) = Direction::eUp;
	}
	else if (inputDirection == InputDirection::eDown)
	{
		// ��ړ�
		mTransform->move.x -= sinf(rotCamera.y) * VALUE_MOVE_MODEL;
		mTransform->move.z -= cosf(rotCamera.y) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = rotCamera.y;
		(*mDirection) = Direction::eDown;
	}
	//else
	//{
	//	// ���͂��Ă��Ȃ���΃j���[�g������
	//	(*mDirection) = Direction::eNeutral;
	//}

	//if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RETURN))
	//{
	//	mTransform->ReSetValue();
	//}
	// �ڕW�p�x�ƌ��݊p�x�Ƃ̍��������߂�
	float diffrot = mDestrot.y - mTransform->rotation.y;
	if (diffrot > XM_PI)
	{
		diffrot -= XM_PI * 2.0f;
	}
	if (diffrot < -XM_PI)
	{
		diffrot += XM_PI * 2.0f;
	}

	// �p�x���x�Ɋ�����t����
	mTransform->rotation.y += diffrot * RATE_ROTATE_MODEL;
	if (mTransform->rotation.y > XM_PI)
	{
		mTransform->rotation.y -= XM_PI * 2.0f;
	}
	if (mTransform->rotation.y < -XM_PI)
	{
		mTransform->rotation.y += XM_PI * 2.0f;
	}

	// �ʒu�ړ�
	mTransform->AddPosition();
	// �ړ��ʂɊ�����������(��������)
	mTransform->move += (mTransform->move*-1.0f) * RATE_MOVE_MODEL;
	// ��]�𔽉f�A���s�ړ��𔽉f
	mTransform->angle = ((mTransform->rotation* 180.0f) / XM_PI);

	// �d�͉�
	mTransform->PositionCorrectionY(-1);
	if (mTransform->position.y < DICE_SCALE_HALF - 3)
	{
		mTransform->position.y = DICE_SCALE_HALF - 3;
	}

	// �㉺���E�ǂ̏���
	// ��
	if (-mStageSize.z*DICE_SCALE + DICE_SCALE_HALF + mTransform->scale.z / 2.0f > mTransform->position.z)
	{
		mTransform->SetPositionZ(-mStageSize.z*DICE_SCALE + DICE_SCALE_HALF + mTransform->scale.z / 2.0f);
		mTransform->move = 0;
	}
	// ��
	if (0 + DICE_SCALE_HALF - mTransform->scale.z / 2.0f < mTransform->position.z)
	{
		mTransform->SetPositionZ(0 + DICE_SCALE_HALF - mTransform->scale.z / 2.0f);
		mTransform->move = 0;
	}
	// �E
	if (mStageSize.x*DICE_SCALE - DICE_SCALE_HALF - mTransform->scale.x / 2.0f < mTransform->position.x)
	{
		mTransform->SetPositionX(mStageSize.x*DICE_SCALE - DICE_SCALE_HALF - mTransform->scale.x / 2.0f);
		mTransform->move = 0;
	}
	// ��
	if (0 - DICE_SCALE_HALF + mTransform->scale.x / 2.0f > mTransform->position.x)
	{
		mTransform->SetPositionX(0 - DICE_SCALE_HALF + mTransform->scale.x / 2.0f);
		mTransform->move = 0;
	}

	mTransform->CreateWordMtx();


	// �����̃}�b�v�ʒu�ɂ���Dice�̃|�C���^�擾
	mpOperationDice = DiceManager::GetInstance()->GetDice(mMapPos);


	// Dice�̗L���ő����̏�ԕύX
	if (!mpOperationDice.IsExist())
	{
		*mFoot = Foot::eFloor;
		CheckPush();
		SetMapPos();
		return;
	}
	//else
	//{
	//	*mFoot = Foot::eDice;
	//}


	if (mpOperationDice->GetTransform()->GetPosition().y < mTransform->position.y)
		*mFoot = Foot::eDice;

	if (*mFoot == Foot::eFloor)
	{
		if (mpOperationDice->GetDiceStatus() != DiceStatus::eNormal)
		{
			mTransform->SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + mTransform->scale.y - 0.5f);
		}
	}
	else if (*mFoot == Foot::eDice)
	{
		if (CheckRoll())
			return;

		Float3 dicePos = (mpOperationDice->GetTransform()->position);
		/// Todo:�������_�C�X�̎��̈ړ����������������(������������~����A�~��悤�Ƃ��Ă��~���Ȃ����̏���)
		switch (mpOperationDice->GetDiceStatus())
		{
		case DiceStatus::eUp:
			break;
		case DiceStatus::eHalfUp:
			// �ړ�����
			if (dicePos.x + DICE_SCALE_HALF + mTransform->scale.x / 2.0f < mTransform->GetPosition().x)
			{
				mTransform->SetPositionX(dicePos.x + DICE_SCALE_HALF + mTransform->scale.x / 2.0f);
				mTransform->move = 0;
			}
			if (dicePos.x - DICE_SCALE_HALF - mTransform->scale.x / 2.0f > mTransform->GetPosition().x)
			{
				mTransform->SetPositionX(dicePos.x - DICE_SCALE_HALF - mTransform->scale.x / 2.0f);
				mTransform->move = 0;
			}
			if (dicePos.z + DICE_SCALE_HALF + mTransform->scale.z / 2.0f < mTransform->GetPosition().z)
			{
				mTransform->SetPositionZ(dicePos.z + DICE_SCALE_HALF + mTransform->scale.z / 2.0f);
				mTransform->move = 0;
			}
			if (dicePos.z - DICE_SCALE_HALF - mTransform->scale.z / 2.0f > mTransform->GetPosition().z)
			{
				mTransform->SetPositionZ(dicePos.z - DICE_SCALE_HALF - mTransform->scale.z / 2.0f);
				mTransform->move = 0;
			}
			break;
		case DiceStatus::eRoll:
			return;
		case DiceStatus::eDown:
			break;
		case DiceStatus::eHalfDown:
			break;
		}
		mTransform->SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + mTransform->scale.y);
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
