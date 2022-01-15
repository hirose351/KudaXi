#include	"access_dice_manager.h"
#include	"../manager/dice_manager.h"

AccessDiceManager::AccessDiceManager() :GameObject(("DiceManager"), ObjectType::eObstracle, true)
{
}

void AccessDiceManager::ObjectInit()
{
	DiceManager::GetInstance()->Init();
}

void AccessDiceManager::ObjectUpdate()
{
	DiceManager::GetInstance()->Update();
}

void AccessDiceManager::ObjectImguiDraw()
{
	DiceManager::GetInstance()->ImguiDraw();
}
