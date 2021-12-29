#include	"easing_easticout_component.h"
#include	"../../system/util/easing.h"
#include	"quad2d_component.h"

using namespace Component;

void EasingUI::EasticOut::Update()
{
	//if (mCurrentTime < 0)
	//{
	//	SetIsActive(false);
	//	return;
	//}

	mScale = Easing::ElasticOut(mCurrentTime, mTotalTime, min, max);
	mCurrentTime -= 1.0f;
	mOwner->GetComponent<Component::Quad2dComponent>()->SetScale(DirectX::XMFLOAT2(mScale, mScale));
}

void EasingUI::EasticOut::SetNum(float time, float _min, float _max)
{
	mTotalTime = time;
	mCurrentTime = mTotalTime;
	min = _min;
	max = _max;
}