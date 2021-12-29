#include "ui_image.h"
#include "../component/quad2d_component.h"

void myUI::Image::ObjectInit()
{
	AddComponent<Component::Quad2dComponent>();
	GetComponent<Component::Quad2dComponent>()->SetInfo(500, 250, "assets/image/ui/Clear.png", XMFLOAT4(1, 1, 1, 1));
}
