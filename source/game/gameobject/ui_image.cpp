#include "ui_image.h"
#include "../component/quad2d_component.h"
#include "../component/easing_component.h"

myUI::Image::Image() :GameObject(("Image"), ObjectType::eObstracle, true)
{
};

void myUI::Image::ObjectInit()
{
}

void myUI::Image::ObjectUpdate()
{
	mTransform->CreateWordMtx();
}
