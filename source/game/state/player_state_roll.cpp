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
		mTransform->SetPositionZ(dicePos.z + DICE_SCALE_HALF - mTransform->scale.z / 2.0f - 1.0f);
		break;
	case Direction::eDown:
		mTransform->SetPositionZ(dicePos.z - DICE_SCALE_HALF + mTransform->scale.z / 2.0f + 1.0f);
		break;
	case Direction::eLeft:
		mTransform->SetPositionX(dicePos.x - DICE_SCALE_HALF + mTransform->scale.x / 2.0f + 1.0f);
		break;
	case Direction::eRight:
		mTransform->SetPositionX(dicePos.x + DICE_SCALE_HALF - mTransform->scale.x / 2.0f - 1.0f);
		break;
	}
	mTransform->SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + mTransform->scale.y);

	if (mpOperationDice->GetDiceStatus() != DiceStatus::eRoll)
	{
		mHolder->ChangeState(eMove);	// ó‘Ô‚ð•Ï‚¦‚é
	}
}

void Roll::BeforeChange()
{
}

void Roll::AfterChange()
{
	Float3 dicePos = mpOperationDice->GetTransform()->GetPosition();
	switch (*mDirection)
	{
	case Direction::eNeutral:
		mHolder->ChangeState(eMove);	// ó‘Ô‚ð•Ï‚¦‚é
		return;
	case Direction::eUp:
		mTransform->SetPositionZ(dicePos.z + DICE_SCALE_HALF - mTransform->scale.z / 2.0f - 1.0f);
		break;
	case Direction::eDown:
		mTransform->SetPositionZ(dicePos.z - DICE_SCALE_HALF + mTransform->scale.z / 2.0f + 1.0f);
		break;
	case Direction::eLeft:
		mTransform->SetPositionX(dicePos.x - DICE_SCALE_HALF + mTransform->scale.x / 2.0f + 1.0f);
		break;
	case Direction::eRight:
		mTransform->SetPositionX(dicePos.x + DICE_SCALE_HALF - mTransform->scale.x / 2.0f - 1.0f);
		break;
	}
	mTransform->move = 0;
}
