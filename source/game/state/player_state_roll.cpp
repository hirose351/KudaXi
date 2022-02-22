#include "player_state_move.h"
#include "player_state_roll.h"

using namespace PlayerState;

void Roll::Exec()
{
	if (!mpOperationDice.IsExist())
	{
		mpHolder->ChangeState(eMove);	// ó‘Ô‚ð•Ï‚¦‚é
		return;
	}
	Float3 dicePos = mpOperationDice->GetTransform()->GetPosition();
	switch (*mpDirection)
	{
	case Direction::eNeutral:
		mpHolder->ChangeState(eMove);	// ó‘Ô‚ð•Ï‚¦‚é
		return;
	case Direction::eUp:
		mpTransform->SetPositionZ(dicePos.z + DICE_SCALE_HALF - mpTransform->scale.z / 2.0f - 1.0f);
		break;
	case Direction::eDown:
		mpTransform->SetPositionZ(dicePos.z - DICE_SCALE_HALF + mpTransform->scale.z / 2.0f + 1.0f);
		break;
	case Direction::eLeft:
		mpTransform->SetPositionX(dicePos.x - DICE_SCALE_HALF + mpTransform->scale.x / 2.0f + 1.0f);
		break;
	case Direction::eRight:
		mpTransform->SetPositionX(dicePos.x + DICE_SCALE_HALF - mpTransform->scale.x / 2.0f - 1.0f);
		break;
	}
	mpTransform->SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + mpTransform->scale.y);

	if (mpOperationDice->GetDiceStatus() != DiceStatus::eRoll)
	{
		mpHolder->ChangeState(eMove);	// ó‘Ô‚ð•Ï‚¦‚é
	}
}

void Roll::BeforeChange()
{
}

void Roll::AfterChange()
{
	if (!mpOperationDice.IsExist())
	{
		mpHolder->ChangeState(eMove);	// ó‘Ô‚ð•Ï‚¦‚é
		return;
	}
	Float3 dicePos = mpOperationDice->GetTransform()->GetPosition();
	switch (*mpDirection)
	{
	case Direction::eNeutral:
		mpHolder->ChangeState(eMove);	// ó‘Ô‚ð•Ï‚¦‚é
		return;
	case Direction::eUp:
		mpTransform->SetPositionZ(dicePos.z + DICE_SCALE_HALF - mpTransform->scale.z / 2.0f - 1.0f);
		break;
	case Direction::eDown:
		mpTransform->SetPositionZ(dicePos.z - DICE_SCALE_HALF + mpTransform->scale.z / 2.0f + 1.0f);
		break;
	case Direction::eLeft:
		mpTransform->SetPositionX(dicePos.x - DICE_SCALE_HALF + mpTransform->scale.x / 2.0f + 1.0f);
		break;
	case Direction::eRight:
		mpTransform->SetPositionX(dicePos.x + DICE_SCALE_HALF - mpTransform->scale.x / 2.0f - 1.0f);
		break;
	}
	mpTransform->move = 0;
}
