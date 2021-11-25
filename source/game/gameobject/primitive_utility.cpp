#include	"primitive_utility.h"

void PrimitiveBase::SetManager()
{
	CollisionManager::GetInstance().AddPrim(this);
}
