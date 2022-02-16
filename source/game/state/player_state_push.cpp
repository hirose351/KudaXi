#include "player_state_move.h"
#include "player_state_push.h"

using namespace PlayerState;

void Push::Exec()
{
	if (mpOperationDice->GetPushEnd())
	{
		mHolder->ChangeState(eMove);	// 状態を変える
		return;
	}

	float movePos = 1;

	switch (*mDirection)
	{
	case Direction::eNeutral:
		mHolder->ChangeState(eMove);	// 状態を変える
		return;
	case Direction::eUp:
		mTransform->move = { 0, 0, movePos };
		break;
	case Direction::eDown:
		mTransform->move = { 0, 0, -movePos };
		break;
	case Direction::eLeft:
		mTransform->move = { -movePos, 0, 0 };
		break;
	case Direction::eRight:
		mTransform->move = { movePos, 0, 0 };
		break;
	}

	mTransform->AddPosition();
	mTransform->CreateWordMtx();
}

void Push::BeforeChange()
{
}

void Push::AfterChange()
{
	mTransform->move = 0;
}
