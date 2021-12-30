#include "ui_image.h"
#include "../component/quad2d_component.h"
#include "../component/easing_image_component.h"

myUI::Image::Image() :GameObject(("Image"), ObjectType::eObstracle)
{
	mTransform->SetScale(Float3(100.0f, 50.0f, 1.0f));
	Component::EasingImage* a = AddComponent<Component::EasingImage>();
	a->AddEasing(Easing::EasingType::eElasticInOut, TransType::ePos, 100.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(500.0f, 250.0f), true);
	a->AddEasing(Easing::EasingType::eExpInOut, TransType::ePos, 100.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(100.0f, 50.0f), true);
};

void myUI::Image::ObjectInit()
{
	AddComponent<Component::Quad2d>();
	GetComponent<Component::Quad2d>()->SetInfo(XMFLOAT2(500, 250), "assets/image/ui/Clear.png", XMFLOAT4(1, 1, 1, 1));
	mTransform->SetPosition(Float3(500, 500, 0));
}

void myUI::Image::ObjectUpdate()
{
	mTransform->CreateMtx();
}
