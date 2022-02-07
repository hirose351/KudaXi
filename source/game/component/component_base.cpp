#include "component_base.h"

bool ComponentBase::GetIsActiveWithOwner()
{
	if (mState != ObjectState::eActive)
		return false;
	if (mOwner->GetParent() == nullptr)
		return mOwner->GetIsActive();
	return (mOwner->GetIsActive() && mOwner->GetParent()->GetIsActive());
}
