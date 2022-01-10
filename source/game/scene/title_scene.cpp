#include	"title_scene.h"
#include	"../gameobject/ui_image.h"
#include	"../component/quad2d_component.h"
#include	"../gameobject/skydome.h"
#include	"../gameobject/ui_button.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::SceneInit()
{
	Skydome* skydome = new Skydome;

	myUI::Image* i = new myUI::Image;
	i->GetTransform()->SetScale(Float3(700, 650, 0));
	i->GetTransform()->SetPositionXYZ(Float3(350, 350, 0));
	i->AddComponent<Component::Quad2d>()->SetInfo("assets/image/title/rogo.png", XMFLOAT4(1, 1, 1, 1));

	myUI::ButtonGroup* bG = new myUI::ButtonGroup;
	bG->GetTransform()->SetPositionXYZ(Float3(950.0f, 300.0f, 0));
	bG->SetInitState("assets/image/title/button.png", 2, 1, 1, ButtonTransition::eColorTint, XMFLOAT2(950.0f, 300.0f), XMFLOAT2(100, 100), XMFLOAT2(150, 150), ButtonArrangement::eVertical);
}

void TitleScene::SceneUpdate()
{
}

void TitleScene::SceneRender()
{
}

bool TitleScene::Dispose()
{
	return false;
}
