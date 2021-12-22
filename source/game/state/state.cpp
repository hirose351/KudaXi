#include "state.h"

void State::Start(Component::PlayerController* _c)
{
	mHolder = _c;
	mTransform = mHolder->GetOwner()->GetTransform();
	mDirection = mHolder->GetDirection();
	mFoot = mHolder->GetFoot();

	Init();
}