#include	"primitive.h"

void PrimitiveBase::SetManager()
{
	CollisionManager::GetInstance().AddPrim(this);
}
