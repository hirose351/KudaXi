#include	"map_pos_component.h"
#include	"../manager/stagedata_manager.h"

using namespace Component;

MapPos::MapPos() :ComponentBase((u8"マップ位置"))
{
}

void MapPos::Init()
{
	// 現在のシーンを取得
	mCurrentStageData = StageDataManager::GetInstance().GetCurrentStage().At();
}

void MapPos::Update()
{

}

void MapPos::ImguiDraw()
{
	ImGui::Text(u8"マップ上の位置");
	std::string str = u8"x：" + std::to_string(mPos.x);
	str += u8"      z：" + std::to_string(mPos.y);
	ImGui::Text(str.c_str());
}

bool MapPos::AddMapPos(INT2 _pos)
{
	if ((mPos + _pos).x < 0 || (mPos + _pos).x >= mCurrentStageData.mMapSizeWidth || (mPos + _pos).y < 0 || (mPos + _pos).y >= mCurrentStageData.mMapSizeHeight)
		return false;

	mPos += _pos;
	return true;
}
