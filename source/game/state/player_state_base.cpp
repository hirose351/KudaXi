#include	"player_state_base.h"

void PlayerStateBase::Start(Component::PlayerController* _c)
{
	mpHolder = _c;
	mpTransform = mpHolder->GetOwner()->GetTransform();
	mpDirection = mpHolder->GetDirection();
	mpFoot = mpHolder->GetFoot();

	Init();
}