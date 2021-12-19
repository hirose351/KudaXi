#include "state.h"

void State::Start(Dix::wp<Component::PlayerController> _c)
{
	mHolder = _c;
	mTramsform = mHolder->GetOwner()->GetTransform();
}
