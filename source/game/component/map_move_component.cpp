#include	"map_move_component.h"
#include	"map_pos_component.h"
#include	"../manager/input_manager.h"
#include	"../manager/dice_manager.h"

using namespace Component;

#define MOVE (0)
#define ROT  (1)

static int transTypeNum;

MapMove::MapMove() :ComponentBase((u8"ˆÚ“®‚Æ‰ñ“]"))
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
	/// Todo:‚Ô‚Ô[‚Ä‚È‚ç‚·

	// ‰Ÿ‚³‚ê‚½uŠÔ‚Ìˆ—
	InputDirection direction = InputManager::GetInstance().GetDirectionTrigger(InputMode::eUi, static_cast<int>(UiAction::eNavigate));

	switch (direction)
	{
	case InputDirection::eUp:
		if (mOwnerType == ObjectType::ePlayer)
			pos->AddMapPos(INT2(0, -1));
		break;
	case InputDirection::eDown:
		if (mOwnerType == ObjectType::ePlayer)
			pos->AddMapPos(INT2(0, 1));
		break;
	case InputDirection::eLeft:
		if (mOwnerType == ObjectType::ePlayer)
			pos->AddMapPos(INT2(-1, 0));
		break;
	case InputDirection::eRight:
		if (mOwnerType == ObjectType::ePlayer)
			pos->AddMapPos(INT2(1, 0));
		break;
	}
}

void MapMove::ImguiDraw()
{
	ImGui::Text(" ");
	ImGui::Text(u8"“®‚©‚·ƒ‚[ƒhØ‚è‘Ö‚¦      Space‚Å‚àØ‚è‘Ö‚¦‰Â");
	ImGui::RadioButton(u8"ˆÚ“®", &transTypeNum, MOVE);
	ImGui::RadioButton(u8"‰ñ“]", &transTypeNum, ROT);
}
