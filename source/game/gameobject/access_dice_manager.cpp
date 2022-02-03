#include	"access_dice_manager.h"
#include	"../manager/dice_manager.h"

DiceManagerAccess::DiceManagerAccess() :GameObject(("DiceManager"), ObjectType::eObstracle, true)
{
}

void DiceManagerAccess::ObjectInit()
{
	//DiceManager::GetInstance()->Init();
}

void DiceManagerAccess::ObjectUpdate()
{
	//DiceManager::GetInstance()->EndleesUpdate();
}

void DiceManagerAccess::ObjectImguiDraw()
{
	DiceManager::GetInstance()->ImguiDraw();
}
