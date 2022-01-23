#include	"map_move_component.h"
#include	"map_pos_component.h"
#include	"../manager/input_manager.h"
#include	"../manager/dice_manager.h"

using namespace Component;

#define MOVE (0)
#define ROT  (1)

static int transTypeNum;

MapMove::MapMove() :ComponentBase((u8"移動と回転"))
{
	SetIsCreate(true);
}

void MapMove::Init()
{
	mOwnerType = mOwner->GetObjectType();
	pos = mOwner->GetComponent<Component::MapPos>();
}

void MapMove::Update()
{
	/// Todo:ぶぶーてならす

	// 押された瞬間の処理
	InputDirection direction = InputManager::GetInstance().GetDirectionTrigger(InputMode::eUi, static_cast<int>(UiAction::eNavigate));

	if (direction == InputDirection::eNeutral)
		return;

	INT2 a;

	switch (direction)
	{
	case InputDirection::eUp:
		a = (INT2(0, -1));
		break;
	case InputDirection::eDown:
		a = (INT2(0, 1));
		break;
	case InputDirection::eLeft:
		a = (INT2(-1, 0));
		break;
	case InputDirection::eRight:
		a = (INT2(1, 0));
		break;
	}
	if (mOwnerType == ObjectType::ePlayer)
	{
		pos->AddMapPos(a);
	}
	else if (mOwnerType == ObjectType::eDice)
	{
		pos->AddMapPos(DiceManager::GetInstance()->GetMoveMapPos(static_cast<Direction>(direction), pos->GetMapPos()));
	}
}

void MapMove::ImguiDraw()
{
	ImGui::Text(" ");
	ImGui::Text(u8"動かすモード切り替え      Spaceでも切り替え可");
	ImGui::RadioButton(u8"移動", &transTypeNum, MOVE);
	ImGui::RadioButton(u8"回転", &transTypeNum, ROT);
}
