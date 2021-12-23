#include	"player_state_base.h"

void PlayerStateBase::Start(Component::PlayerController* _c)
{
	mHolder = _c;
	mTransform = mHolder->GetOwner()->GetTransform();
	mDirection = mHolder->GetDirection();
	mFoot = mHolder->GetFoot();

	Init();
}