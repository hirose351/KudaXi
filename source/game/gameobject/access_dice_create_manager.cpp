#include	"access_dice_create_manager.h"
#include	"../manager/dice_manager.h"

DiceCreateManagerAccess::DiceCreateManagerAccess() :GameObject(("DiceManager"), ObjectType::eObstracle, true)
{
}

void DiceCreateManagerAccess::ObjectInit()
{
	DiceManager::GetInstance()->CreateInit();
}

void DiceCreateManagerAccess::ObjectUpdate()
{
	DiceManager::GetInstance()->CreateUpdate();
}

void DiceCreateManagerAccess::ImguiCreateDraw()
{
	DiceManager::GetInstance()->CreateImguiDraw();
}
