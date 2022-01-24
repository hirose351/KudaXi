#include	"map_pos_component.h"
#include	"../manager/stagedata_manager.h"

using namespace Component;

void MapPos::SetPos()
{
	mOwner->GetTransform()->SetPositionX(mPos.x*DICE_SCALE);
	mOwner->GetTransform()->SetPositionZ(-mPos.z*DICE_SCALE);
}

MapPos::MapPos() :ComponentBase((u8"�}�b�v��̈ʒu"))
{
	SetIsCreate(true);
	mPos = { 0,0 };
}

void MapPos::Init()
{
	// ���݂̃V�[�����擾
	mCurrentStageData = StageDataManager::GetInstance().GetCurrentStage().At();
	SetPos();
}

void MapPos::Update()
{

}

void MapPos::ImguiDraw()
{
	std::string str = u8"x�F" + std::to_string(mPos.x);
	str += u8"      z�F" + std::to_string(mPos.z);
	ImGui::Text(str.c_str());
}

bool MapPos::AddMapPos(INT2 _pos)
{
	if ((mPos + _pos).x < 0 || (mPos + _pos).x >= mCurrentStageData.mMapSizeWidth || (mPos + _pos).z < 0 || (mPos + _pos).z >= mCurrentStageData.mMapSizeHeight)
		return false;

	mPos += _pos;
	SetPos();
	return true;
}
