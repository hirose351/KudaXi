#include	"map_move_component.h"
#include	"map_pos_component.h"
#include	"../manager/input_manager.h"
#include	"../manager/dice_manager.h"

using namespace Component;

#define MOVE (0)
#define ROT  (1)

static int transTypeNum;

MapMove::MapMove() :ComponentBase((u8"マップ上の操作"))
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
	if (transTypeNum == 0)
	{
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
	else if (mOwnerType == ObjectType::eDice)
	{
		DirectX::XMFLOAT4X4 rotMtx;
		Float3 rot = 0;
		switch (direction)
		{
		case InputDirection::eUp:
			DX11MtxRotationX(90, rotMtx);
			break;
		case InputDirection::eDown:
			DX11MtxRotationX(-90, rotMtx);
			break;
		case InputDirection::eLeft:
			DX11MtxRotationZ(-90, rotMtx);
			break;
		case InputDirection::eRight:
			DX11MtxRotationZ(90, rotMtx);
			break;
		}
		DX11MtxMultiply(rotMtx, rotMtx, mOwner->GetTransform()->GetMtx());
		mOwner->GetTransform()->SetWordMtx(rotMtx);
	}
}

void MapMove::ImguiDraw()
{
	ImGui::Text(u8"↑←↓→ or WASD");
	if (mOwnerType == ObjectType::ePlayer)
		return;
	ImGui::Text(" ");
	ImGui::Text(u8"動かすモード切り替え  Spaceでも切り替え可");
	ImGui::RadioButton(u8"移動", &transTypeNum, MOVE);
	ImGui::SameLine();
	ImGui::RadioButton(u8"回転", &transTypeNum, ROT);
}
