#include "player_controller.h"

using namespace Component;

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::Init()
{
}

void PlayerController::Update()
{
	mStates[mStatesNum]->Exec();
}

void PlayerController::Draw()
{
}

void PlayerController::ImguiDraw()
{
}
