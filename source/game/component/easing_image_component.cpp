#include	"easing_image_component.h"
#include	"quad2d_component.h"

using namespace Component;

void EasingImage::Update()
{
	if (mEasingList.empty())
		return;

	// Easingƒ^ƒCƒv‚ðŒˆ‚ß‚é
	EasingFamily famly = mEasingList.front();
	XMFLOAT2 ansValue;

	if (!isStart)
	{
		switch (famly.transType)
		{
		case TransType::ePos:
			mStartValue = { mOwner->GetTransform()->GetPosition().x ,mOwner->GetTransform()->GetPosition().y };
			break;
		case TransType::eRot:
			break;
		case TransType::eScale:
			break;
		}
		mCurrentTime = 0;
		isStart = true;
	}

	ansValue.x = Easing::GetEsingAns(famly.easingType, mCurrentTime, famly.totalTime, famly.startValue.x, famly.endValue.x);
	ansValue.y = Easing::GetEsingAns(famly.easingType, mCurrentTime, famly.totalTime, famly.startValue.y, famly.endValue.y);

	std::cout << "ansValue.x:" << ansValue.x << "ansValue.y:" << ansValue.y << "\n";

	switch (famly.transType)
	{
	case TransType::ePos:
		break;
	case TransType::eRot:
		break;
	case TransType::eScale:
		mOwner->GetComponent<Component::Quad2d>()->SetScale(ansValue);
		break;
	}

	mCurrentTime += 1.0f;

	if (mCurrentTime < famly.totalTime)
		return;

	mEasingList.pop_front();
	isStart = false;
}

void EasingImage::AddEasing(Easing::EasingType _easingType, TransType _transType, float _totalTime, DirectX::XMFLOAT2 _startValue, DirectX::XMFLOAT2 _endValue)
{
	EasingFamily newFamily;
	newFamily.easingType = _easingType;
	newFamily.transType = _transType;
	newFamily.totalTime = _totalTime;
	newFamily.startValue = _startValue;
	newFamily.endValue = _endValue;
	mEasingList.push_back(newFamily);
}