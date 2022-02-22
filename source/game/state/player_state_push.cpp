#include "player_state_move.h"
#include "player_state_push.h"

using namespace PlayerState;

void Push::Exec()
{
	if (mpOperationDice->GetPushEnd())
	{
		mpHolder->ChangeState(eMove);	// ó‘Ô‚ð•Ï‚¦‚é
		return;
	}

	float movePos = 1;

	switch (*mpDirection)
	{
	case Direction::eNeutral:
		mpHolder->ChangeState(eMove);	// ó‘Ô‚ð•Ï‚¦‚é
		return;
	case Direction::eUp:
		mpTransform->move = { 0, 0, movePos };
		break;
	case Direction::eDown:
		mpTransform->move = { 0, 0, -movePos };
		break;
	case Direction::eLeft:
		mpTransform->move = { -movePos, 0, 0 };
		break;
	case Direction::eRight:
		mpTransform->move = { movePos, 0, 0 };
		break;
	}

	mpTransform->AddPosition();
	mpTransform->CreateWordMtx();
}

void Push::BeforeChange()
{
}

void Push::AfterChange()
{
	mpTransform->move = 0;
}
