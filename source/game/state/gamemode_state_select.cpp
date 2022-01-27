#include	"gamemode_state_select.h"
#include	"../gameobject/ui_image.h"
#include	"../component/quad2d_component.h"

using namespace GameModeState;

Select::Select()
{
	Dix::sp<myUI::Image> diceBg;
	diceBg.SetPtr(new myUI::Image);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(diceBg);
	Component::Quad2d* bgQuad = diceBg->AddComponent<Component::Quad2d>();
	bgQuad->SetInfo("", XMFLOAT4(1, 1, 1, 0.8f));
	bgQuad->SetDrawType(DrawType::eNoTex);
	bgQuad->SetOrderInLayer(1);
	diceBg->GetTransform()->SetScale((188));
	diceBg->GetTransform()->SetPosition(Float3(150));
	diceBg->GetTransform()->CreateWordMtx();

	Dix::sp<myUI::ButtonGroup> b;
	b.SetPtr(new myUI::ButtonGroup);
	b->GetTransform()->SetPositionXYZ(Float3(600, 300, 0));
	b->SetInitState("assets/image/ui/number.png", 9, 1, 1, ButtonTransition::eColorTint, XMFLOAT2(0, 0), XMFLOAT2(200, 200), XMFLOAT2(300, 300), ButtonArrangement::eVertical);
	b->SetIsActive(false);
	mButton = b;
	mButton->SetInitSelectNum(0);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(b);
}

Select::~Select()
{
}

void Select::Exec()
{
}

void Select::BeforeChange()
{
}

void Select::AfterChange()
{
}
