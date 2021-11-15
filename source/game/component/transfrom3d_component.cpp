#include "transform3d_component.h"

using namespace Component;

void Transform3d::CheckMoveMode()
{
	//移動モードが力によって動く状態（Force）の時は移動量をforceで管理する。
	//このときdirectionのx,yそれぞれの値が1を超えていると意図しない移動になってしまうため、directionを正規化しておく

	if (moveMode == MoveMode::Volume)
		return;

	if (rotation.x + rotation.y + rotation.z > 1.41421356237)//ココ正しくは√2なんだけどめんどくさいからコレで
		rotation.Normalize();
}

void Transform3d::Update()
{
	CheckMoveMode();

	switch (moveMode)
	{
	case MoveMode::Volume:
		MoveByVolume();
		break;

	case MoveMode::Force:
		MoveByForce();
		break;
	}
}