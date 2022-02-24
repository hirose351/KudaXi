#include	"effect_thunder.h"
#include	"../component/billbord_component.h"
#include	"../component/easing_component.h"

using namespace Effect;
using namespace DirectX;

void Thunder::ObjectInit()
{
	Component::Billbord* bilbord = AddComponent<Component::Billbord>();
	bilbord->LoadTexture("assets/image/effect/thunder/thunder_yellow.png");
	bilbord->SetScale(XMFLOAT2(100.0f, 200.0f));
	bilbord->SetDivUV(XMFLOAT2(2, 1));
	bilbord->SetUV(XMFLOAT2(0, 0));
	bilbord->SetOrderInLayer(80);
	mTransform->SetPositionXYZ(mInitPos);
	AddComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::eBilbordAlha, 40, 0, Float3(1), Float3(0));
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

	if (GetComponent<Component::Billbord>()->GetAlha() <= 0)
		mObjectState = ObjectState::eDead;
}
