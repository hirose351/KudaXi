#include	"map_move_component.h"

using namespace Component;

#define MOVE (0)
#define ROT  (1)

static int transTypeNum;

MapMove::MapMove() :ComponentBase((u8"移動と回転"))
{
}

void MapMove::Init()
{
}

void MapMove::Update()
{
}

void MapMove::ImguiDraw()
{
	ImGui::Text(u8"マップ上の位置");
	//std::string str = u8"x：" + std::to_string(mStage.GetDiceMapPos(mGameObjectNum - mDiceNum).x);
	//str += u8"      z：" + std::to_string(mStage.GetDiceMapPos(mGameObjectNum - mDiceNum).z);
	//ImGui::Text(str.c_str());

	ImGui::Text(" ");
	ImGui::Text(u8"動かすモード切り替え      Spaceでも切り替え可");
	ImGui::RadioButton(u8"移動", &transTypeNum, MOVE);
	ImGui::RadioButton(u8"回転", &transTypeNum, ROT);
	ImGui::TreePop();
}
