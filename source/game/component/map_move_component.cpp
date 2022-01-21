#include	"map_move_component.h"

using namespace Component;

#define MOVE (0)
#define ROT  (1)

static int transTypeNum;

MapMove::MapMove() :ComponentBase((u8"�ړ��Ɖ�]"))
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
	ImGui::Text(u8"�}�b�v��̈ʒu");
	//std::string str = u8"x�F" + std::to_string(mStage.GetDiceMapPos(mGameObjectNum - mDiceNum).x);
	//str += u8"      z�F" + std::to_string(mStage.GetDiceMapPos(mGameObjectNum - mDiceNum).z);
	//ImGui::Text(str.c_str());

	ImGui::Text(" ");
	ImGui::Text(u8"���������[�h�؂�ւ�      Space�ł��؂�ւ���");
	ImGui::RadioButton(u8"�ړ�", &transTypeNum, MOVE);
	ImGui::RadioButton(u8"��]", &transTypeNum, ROT);
	ImGui::TreePop();
}
