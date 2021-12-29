#include	"effect_thunder.h"
#include	"../component/billbord_component.h"

void Effect::Thunder::ObjectInit()
{
	Component::Billbord* bilbord = AddComponent<Component::Billbord>();
	bilbord->LoadTexture("assets/image/effect/thunder/thunder_yellow.png");
	bilbord->SetScale(XMFLOAT2(100.0f, 200.0f));
	bilbord->SetDivUV(XMFLOAT2(2, 1));
	bilbord->SetUV(XMFLOAT2(0, 0));
	mTransform->SetPositionXYZ(mInitPos);
}

void Effect::Thunder::ObjectUpdate()
{
	Component::Billbord* bilbord = GetComponent<Component::Billbord>();

	if (bilbord == nullptr)
		return;

	if (mAlha <= 0.8f)
	{
		bilbord->SetUV(XMFLOAT2(1, 0));
	}
	if (mAlha > 0.0f)
	{
		bilbord->SetColor(XMFLOAT4(1, 1, 1, mAlha));
		mAlha -= 0.025f;
	}
	else
	{
		SetObjectState(ObjectState::eDead);
	}
}
