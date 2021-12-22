#include "player_state_move.h"
#include "player_state_roll.h"

using namespace PlayerState;

void Roll::Exec()
{
	if (mpOperationDice == nullptr)
	{
		mHolder->ChangeState(eMove);	// ó‘Ô‚ð•Ï‚¦‚é
		return;
	}
	Float3 dicePos = mpOperationDice->GetTransform()->GetPosition();
	switch (*mDirection)
	{
	case Direction::eNeutral:
		mHolder->ChangeState(eMove);	// ó‘Ô‚ð•Ï‚¦‚é
		return;
	case Direction::eUp:
		mTransform->SetPositionZ(dicePos.z + DICE_SCALE_HALF - mTransform->scale.z);
		break;
	case Direction::eDown:
		mTransform->SetPositionZ(dicePos.z - DICE_SCALE_HALF + mTransform->scale.z);
		break;
	case Direction::eLeft:
		mTransform->SetPositionX(dicePos.x - DICE_SCALE_HALF + mTransform->scale.x);
		break;
	case Direction::eRight:
		mTransform->SetPositionX(dicePos.x + DICE_SCALE_HALF - mTransform->scale.x);
		break;
	}
	mTransform->SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE + mTransform->scale.y);

	if (mpOperationDice->GetDiceStatus() != DiceStatus::eRoll)
	{
		mMapPos.x = static_cast<int>((mTransform->position.x + DICE_SCALE_HALF) / DICE_SCALE);
		mMapPos.z = static_cast<int>((mTransform->position.z - DICE_SCALE_HALF) / DICE_SCALE) * -1;

		if (mMapPos.x < 0)
			mMapPos.x = 0;
		if (mMapPos.z < 0)
			mMapPos.z = 0;
		mHolder->ChangeState(eMove);	// ó‘Ô‚ð•Ï‚¦‚é
	}
}

void Roll::BeforeChange()
{
}

void Roll::AfterChange()
{
	*mDirection = Direction::eNeutral;
	mTransform->move = 0;
}
