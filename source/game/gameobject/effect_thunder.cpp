#include	"effect_thunder.h"
#include	"../component/billbord_component.h"

using namespace Effect;
using namespace DirectX;

void Thunder::ObjectInit()
{
	Component::Billbord* bilbord = AddComponent<Component::Billbord>();
	bilbord->LoadTexture("assets/image/effect/thunder/thunder_yellow.png");
	bilbord->SetScale(XMFLOAT2(100.0f, 200.0f));
	bilbord->SetDivUV(XMFLOAT2(2, 1));
	bilbord->SetUV(XMFLOAT2(0, 0));
	bilbord->SetOrderInLayer(10);
	mTransform->SetPositionXYZ(mInitPos);
}

void Thunder::ObjectUpdate()
{
	if (SceneManager::GetInstance()->GetGameMode() != GameMode::eEndless)
		SetObjectState(ObjectState::eDead);

	if (SceneManager::GetInstance()->GetCurrentScene()->GetIsPause())
		return;

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
		mAlha -= mAlhaPerFrame;
	}
	else
	{
		SetObjectState(ObjectState::eDead);
	}
}
