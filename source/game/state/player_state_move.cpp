#include	"player_state_move.h"
#include	"../../system/dx11/CDirectInput.h"
#include	"../manager/dice_manager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_MOVE_MODEL	(0.085f)				// �ړ����x
#define	VALUE_ROTATE_MODEL	(XM_PI * 0.025f)		// ��]���x
#define	RATE_ROTATE_MODEL	(0.10f)					// ��]�����W��
#define	RATE_MOVE_MODEL		(0.1f)

using namespace PlayerState;

void Move::CheckRoll()
{
	if (mpOperationDice == nullptr || *mFoot != Foot::eDice)
		return;
	if (mpOperationDice->GetDiceStatus() != DiceStatus::eNormal)
		return;
	Float3 basePoint = mpOperationDice->GetTransform()->GetPosition();	// ��_

	// Dice�̊O���Ɍ������Ă��Ȃ���ΕԂ�
	switch (*mDirection)
	{
	case Direction::eNeutral:
		return;
	case Direction::eUp:
		if (basePoint.z + DICE_SCALE_HALF - mTransform->scale.z >= mTransform->position.z)
			return;
		break;
	case Direction::eDown:
		if (basePoint.z - DICE_SCALE_HALF + mTransform->scale.z <= mTransform->position.z)
			return;
		break;
	case Direction::eLeft:
		if (basePoint.x - DICE_SCALE_HALF + mTransform->scale.x <= mTransform->position.x)
			return;
		break;
	case Direction::eRight:
		if (basePoint.x + DICE_SCALE_HALF - mTransform->scale.x >= mTransform->position.x)
			return;
		break;
	}

	// ��]�\�ʒu�ɂ�����
	if (mpOperationDice->SetRollAction(*mDirection))
	{
		mHolder->ChangeState(eRoll);	// ��Ԃ�ς���
		mTransform->SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE + mTransform->scale.y);
	}
	// ��]�o���Ȃ����
	else
	{
		/// Todo:�s���悪�����������̈ړ�����
		if (!mpOperationDice->CheckDiceDirection(*mDirection))
			return;
		switch (*mDirection)
		{
		case Direction::eUp:
			mTransform->SetPositionZ(basePoint.z + DICE_SCALE_HALF - mTransform->scale.z);
			break;
		case Direction::eDown:
			mTransform->SetPositionZ(basePoint.z - DICE_SCALE_HALF + mTransform->scale.z);
			break;
		case Direction::eLeft:
			mTransform->SetPositionX(basePoint.x - DICE_SCALE_HALF + mTransform->scale.x);
			break;
		case Direction::eRight:
			mTransform->SetPositionX(basePoint.x + DICE_SCALE_HALF - mTransform->scale.x);
			break;
		}
	}
}

void Move::CheckPush()
{
	if (*mFoot != Foot::eFloor)
		return;

	INT3 checkMapPos;

	// �ړ���������̃}�b�v�Ɉړ��\��
	switch (*mDirection)
	{
	case Direction::eNeutral:
		return;
	case Direction::eUp:
		if (-mMapPos.z*DICE_SCALE + DICE_SCALE_HALF - mTransform->scale.z > mTransform->position.z)
			return;
		checkMapPos = INT3(mMapPos.x, 0, mMapPos.z - 1);
		break;
	case Direction::eDown:
		if (-mMapPos.z*DICE_SCALE - DICE_SCALE_HALF + mTransform->scale.z < mTransform->position.z)
			return;
		checkMapPos = INT3(mMapPos.x, 0, mMapPos.z + 1);
		break;
	case Direction::eLeft:
		if (mMapPos.x*DICE_SCALE - DICE_SCALE_HALF + mTransform->scale.z < mTransform->position.x)
			return;
		checkMapPos = INT3(mMapPos.x - 1, 0, mMapPos.z);
		break;
	case Direction::eRight:
		if (mMapPos.x*DICE_SCALE + DICE_SCALE_HALF - mTransform->scale.z > mTransform->position.x)
			return;
		checkMapPos = INT3(mMapPos.x + 1, 0, mMapPos.z);
		break;
	}

	// �ړ�������Dice�����邩
	Dice* checkDice = DiceManager::GetInstance()->GetDice(checkMapPos);

	if (checkDice == nullptr)
	{
		return;
	}

	switch (checkDice->GetDiceStatus())
	{
	case DiceStatus::eNormal:// Dice�̏�Ԃ��ʏ�Ȃ牟��
		mpOperationDice = checkDice;
		if (mpOperationDice->SetPushAction(*mDirection))
		{
			mpOperationDice = checkDice;
			mHolder->ChangeState(ePush);	// ��Ԃ�ς���
			return;
		}
		break;
	case DiceStatus::eUp:
		return;
	case DiceStatus::eHalfUp:
		return;
	case DiceStatus::eHalfDown:
		return;
	case DiceStatus::eDown:
		return;
	}

	// �ړ�����
	switch (*mDirection)
	{
	case Direction::eUp:
		mTransform->SetPositionZ(-mMapPos.z*DICE_SCALE + DICE_SCALE_HALF - mTransform->scale.z);
		break;
	case Direction::eDown:
		mTransform->SetPositionZ(-mMapPos.z*DICE_SCALE - DICE_SCALE_HALF + mTransform->scale.z);
		break;
	case Direction::eLeft:
		mTransform->SetPositionX(mMapPos.x*DICE_SCALE - DICE_SCALE_HALF + mTransform->scale.x);
		break;
	case Direction::eRight:
		mTransform->SetPositionX(mMapPos.x*DICE_SCALE + DICE_SCALE_HALF - mTransform->scale.x);
		break;
	}
	return;
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
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
	{
		// ���ړ�
		radian = rotCamera.y + XM_PI * 0.50f;
		mTransform->move.x -= sinf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
		mTransform->move.z -= cosf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = radian;
		(*mDirection) = Direction::eLeft;
	}
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
	{
		// �E�ړ�
		radian = rotCamera.y - XM_PI * 0.50f;
		mTransform->move.x -= sinf(radian) * VALUE_MOVE_MODEL;
		mTransform->move.z -= cosf(radian) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = radian;
		(*mDirection) = Direction::eRight;
	}
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
	{
		// �O�ړ�
		mTransform->move.x -= sinf(XM_PI) * VALUE_MOVE_MODEL;
		mTransform->move.z -= cosf(XM_PI) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = rotCamera.y + XM_PI;
		(*mDirection) = Direction::eUp;
	}
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
	{
		// ��ړ�
		mTransform->move.x -= sinf(rotCamera.y) * VALUE_MOVE_MODEL;
		mTransform->move.z -= cosf(rotCamera.y) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = rotCamera.y;
		(*mDirection) = Direction::eDown;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RETURN))
	{
		mTransform->ReSetValue();
	}
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
	if (-mStageSize.z*DICE_SCALE + DICE_SCALE_HALF + mTransform->scale.z > mTransform->position.z)
		mTransform->SetPositionZ(-mStageSize.z*DICE_SCALE + DICE_SCALE_HALF + mTransform->scale.z);
	if (0 + DICE_SCALE_HALF - mTransform->scale.z < mTransform->position.z)
		mTransform->SetPositionZ(0 + DICE_SCALE_HALF - mTransform->scale.z);
	if (mStageSize.x*DICE_SCALE - DICE_SCALE_HALF - mTransform->scale.x < mTransform->position.x)
		mTransform->SetPositionX(mStageSize.x*DICE_SCALE - DICE_SCALE_HALF - mTransform->scale.x);
	if (0 - DICE_SCALE_HALF + mTransform->scale.x > mTransform->position.x)
		mTransform->SetPositionX(0 - DICE_SCALE_HALF + mTransform->scale.x);

	mTransform->CreateMtx();


	SetMapPos();


	// �����̃}�b�v�ʒu�ɂ���Dice�̃|�C���^�擾
	mpOperationDice = DiceManager::GetInstance()->GetDice(mMapPos);

	// Dice�̗L���ő����̏�ԕύX
	if (mpOperationDice == nullptr)
	{
		*mFoot = Foot::eFloor;
		CheckPush();
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
		CheckRoll();
		Float3 dicePos = (mpOperationDice->GetTransform()->position);
		/// Todo:�������_�C�X�̎��̈ړ����������������(������������~����A�~��悤�Ƃ��Ă��~���Ȃ����̏���)
		switch (mpOperationDice->GetDiceStatus())
		{
		case DiceStatus::eUp:
			break;
		case DiceStatus::eHalfUp:
			// �ړ�����
			if (dicePos.x + DICE_SCALE_HALF - mTransform->scale.x < mTransform->GetPosition().x)
				mTransform->SetPositionX(dicePos.x + DICE_SCALE_HALF - mTransform->scale.x);
			if (dicePos.x - DICE_SCALE_HALF + mTransform->scale.x > mTransform->GetPosition().x)
				mTransform->SetPositionX(dicePos.x - DICE_SCALE_HALF + mTransform->scale.x);
			if (dicePos.z + DICE_SCALE_HALF - mTransform->scale.z < mTransform->GetPosition().z)
				mTransform->SetPositionZ(dicePos.z + DICE_SCALE_HALF - mTransform->scale.z);
			if (dicePos.z - DICE_SCALE_HALF + mTransform->scale.z > mTransform->GetPosition().z)
				mTransform->SetPositionZ(dicePos.z - DICE_SCALE_HALF + mTransform->scale.z);
			break;
		case DiceStatus::eRoll:
			return;
		case DiceStatus::eDown:
			break;
		case DiceStatus::eHalfDown:
			break;
		}
		mTransform->SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + mTransform->scale.y + 10);
	}
}

void Move::BeforeChange()
{
	SetMapPos();
}

void Move::AfterChange()
{
}
