#include	"access_dice_endless_manager.h"
#include	"../manager/dice_manager.h"

DiceEndlessManagerAccess::DiceEndlessManagerAccess() :GameObject(("DiceManager"), ObjectType::eObstracle, true)
{
}

void DiceEndlessManagerAccess::ObjectInit()
{
	DiceManager::GetInstance()->EndlessInit();
}

void DiceEndlessManagerAccess::ObjectUpdate()
{
	DiceManager::GetInstance()->EndleesUpdate();
}

void DiceEndlessManagerAccess::ObjectImguiDraw()
{
	DiceManager::GetInstance()->ImguiDraw();
}
