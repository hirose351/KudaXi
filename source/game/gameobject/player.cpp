#include "player.h"
#include "../component/allcomponents.h"

Component::Player::Player()
{
	using namespace Component;
	mOwner->AddComponent<Transform3d>();
}
