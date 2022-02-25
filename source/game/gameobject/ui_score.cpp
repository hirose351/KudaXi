#include	"ui_score.h"
#include	"../manager/score_manager.h"
#include	"../component/quad2d_component.h"
#include	"../component/number_component.h"

#define SCOREDIGIT (6)

using namespace myUI;

Score::Score() :GameObject(("ScoreEffect"), ObjectType::eObstracle, false)
{
	mTransform->SetPositionXYZ(Float3(320, 30, 0));
	mTransform->SetScale(Float3(50));
	Component::Quad2d* stepNumQuad = AddComponent<Component::Quad2d>();
	stepNumQuad->SetInfo("assets/image/ui/number_score.png", XMFLOAT4(1, 1, 1, 1), 10);
	stepNumQuad->SetOrderInLayer(100);
	AddComponent<Component::Number>()->Init();
}

void Score::ObjectInit()
{
	ScoreManager::GetInstance()->Init();
	GetComponent<Component::Number>()->SetNum(0, 0, SCOREDIGIT);
}

void Score::ObjectUpdate()
{
	ScoreManager::GetInstance()->Update();
	GetComponent<Component::Number>()->SetNum(ScoreManager::GetInstance()->GetDrawScore(), 0, SCOREDIGIT);
}

void Score::ObjectImguiDraw()
{
	std::string str = u8"DrawScoreF" + std::to_string(ScoreManager::GetInstance()->GetDrawScore());
	ImGui::Text(str.c_str());
	str = u8"ScoreF" + std::to_string(ScoreManager::GetInstance()->GetScore());
	ImGui::Text(str.c_str());
}