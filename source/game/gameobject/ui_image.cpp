#include "ui_image.h"
#include "../component/quad2d_component.h"
#include "../component/easing_image_component.h"

myUI::Image::Image() :GameObject(("Image"), ObjectType::eObstracle)
{
	AddComponent<Component::EsingImage>()->SetNum(100.0f, 0.0f, 500.0f);
};

void myUI::Image::ObjectInit()
{
	AddComponent<Component::Quad2d>();
	GetComponent<Component::Quad2d>()->SetInfo(XMFLOAT2(500, 250), "assets/image/ui/Clear.png", XMFLOAT4(1, 1, 1, 1));
	mTransform->SetPositionXYZ(Float3(500, 500, 0));
}

void myUI::Image::ObjectUpdate()
{
}
