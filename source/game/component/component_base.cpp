#include "component_base.h"

bool ComponentBase::GetIsActiveWithOwner()
{
	if (mOwner->GetParent() == nullptr)
		return (mIsActive && mOwner->GetIsActive());
	return (mIsActive && mOwner->GetIsActive() && mOwner->GetParent()->GetIsActive());
}
