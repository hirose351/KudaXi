#include	"easing_image_component.h"
#include	"../../system/util/easing.h"
#include	"quad2d_component.h"

using namespace Component;

void EsingImage::Update()
{
	//if (mCurrentTime < 0)
	//{
	//	SetIsActive(false);
	//	return;
	//}

	mScale = Easing::ElasticOut(mCurrentTime, mTotalTime, mMin, mMax);
	mCurrentTime -= 1.0f;
	mOwner->GetComponent<Component::Quad2d>()->SetScale(DirectX::XMFLOAT2(mScale, mScale));
}

void EsingImage::SetNum(float _time, float _min, float _max)
{
	mTotalTime = _time;
	mCurrentTime = mTotalTime;
	mMin = _min;
	mMax = _max;
}