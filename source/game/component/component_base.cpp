#include "component_base.h"

bool ComponentBase::GetIsActiveWithOwner()
{
	if (mState != ObjectState::eActive)
		return false;
	if (mpOwner->GetParent() == nullptr)
		return mpOwner->GetIsActive();
	return (mpOwner->GetIsActive() && mpOwner->GetParent()->GetIsActive());
}
