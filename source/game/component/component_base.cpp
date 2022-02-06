#include "component_base.h"

bool ComponentBase::GetIsActiveWithOwner()
{
	if (mOwner == NULL)
		return false;
	if (mOwner->GetParent() == nullptr)
		return (mState == ObjectState::eActive && mOwner->GetIsActive());
	return (mState == ObjectState::eActive && mOwner->GetIsActive() && mOwner->GetParent()->GetIsActive());
}
