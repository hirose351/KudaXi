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
	mOwnerType = mpOwner->GetObjectType();
	mpMapPos = mpOwner->GetComponent<Component::MapPos>();
}

void MapMove::Update()
{
	/// Todo:ぶぶーてならす

	// 押された瞬間の処理
	InputDirection direction = InputManager::GetInstance().GetDirectionTrigger(InputMode::eUi, static_cast<int>(UiAction::eNavigate));

	if (direction == InputDirection::eNeutral)
		return;

	INT2 moveP;
	if (transTypeNum == 0 || mOwnerType == ObjectType::ePlayer)
	{
		switch (direction)
		{
		case InputDirection::eUp:
			moveP = (INT2(0, -1));
			break;
		case InputDirection::eDown:
			moveP = (INT2(0, 1));
			break;
		case InputDirection::eLeft:
			moveP = (INT2(-1, 0));
			break;
		case InputDirection::eRight:
			moveP = (INT2(1, 0));
			break;
		}
		if (mOwnerType == ObjectType::ePlayer)
		{
			mpMapPos->AddMapPos(moveP);
		}
		else if (mOwnerType == ObjectType::eDice)
		{
			mpMapPos->AddMapPos(DiceManager::GetInstance()->GetMoveMapPos(static_cast<Direction>(direction), mpMapPos->GetMapPos()));
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
		DX11MtxMultiply(rotMtx, rotMtx, mpOwner->GetTransform()->GetMtx());
		mpOwner->GetTransform()->SetWordMtx(rotMtx);
	}
}

void MapMove::ImguiDraw()
{
	ImGui::Text(u8"↑←↓→ or WASD");
	if (mOwnerType == ObjectType::ePlayer)
		return;
	// 動かすタイプ切り替え処理
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eSwitch)))
		transTypeNum = (transTypeNum + 1) % 2;
	ImGui::Text(" ");
	ImGui::Text(u8"動かすモード切り替え  Space or RTでも切り替え可");
	ImGui::RadioButton(u8"移動", &transTypeNum, MOVE);
	ImGui::SameLine();
	ImGui::RadioButton(u8"回転", &transTypeNum, ROT);
}
