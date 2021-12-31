#include	"easing_image_component.h"
#include	"quad2d_component.h"

using namespace Component;

EasingImage::EasingImage() :ComponentBase(("EasingImage"))
{
}

void EasingImage::Update()
{
	if (mEasingList.empty())
		return;

	// Easingタイプを決める
	EasingFamily famly = mEasingList.front();

	if (!isStart)
	{
		if (famly.isStartAbsolute)
		{
			switch (famly.transType)
			{
			case TransType::ePos:
				mEasingList.front().startValue = XMFLOAT2(mOwner->GetTransform()->GetPosition().x, mOwner->GetTransform()->GetPosition().y);
				break;
			case TransType::eRot:
				mEasingList.front().startValue = XMFLOAT2(mOwner->GetTransform()->GetAngle().x, mOwner->GetTransform()->GetAngle().y);
				break;
			case TransType::eScale:
				mEasingList.front().startValue = XMFLOAT2(mOwner->GetTransform()->GetScale().x, mOwner->GetTransform()->GetScale().y);
				break;
			}
		}
		if (famly.isEndAbsolute)
		{
			switch (famly.transType)
			{
			case TransType::ePos:
				mEasingList.front().endValue = XMFLOAT2(mOwner->GetTransform()->GetPosition().x, mOwner->GetTransform()->GetPosition().y);
				break;
			case TransType::eRot:
				mEasingList.front().endValue = XMFLOAT2(mOwner->GetTransform()->GetAngle().x, mOwner->GetTransform()->GetAngle().y);
				break;
			case TransType::eScale:
				mEasingList.front().endValue = XMFLOAT2(mOwner->GetTransform()->GetScale().x, mOwner->GetTransform()->GetScale().y);
				break;
			}
		}

		mCurrentTime = 0;
		isStart = true;
	}

	// リストの先頭から値を求める
	famly = mEasingList.front();
	XMFLOAT2 ansValue;

	if (famly.startValue.x == famly.endValue.x)
		ansValue.x = famly.startValue.x;
	else
		ansValue.x = Easing::GetEsingAns(famly.easingType, mCurrentTime, famly.totalTime, famly.startValue.x, famly.endValue.x);

	if (famly.startValue.y == famly.endValue.y)
		ansValue.y = famly.startValue.y;
	else
		ansValue.y = Easing::GetEsingAns(famly.easingType, mCurrentTime, famly.totalTime, famly.startValue.y, famly.endValue.y);

	switch (famly.transType)
	{
	case TransType::ePos:
		mOwner->GetTransform()->SetPosition(Float3(ansValue.x, ansValue.y, 1.0f));
		break;
	case TransType::eRot:
		mOwner->GetTransform()->SetAngle(Float3(ansValue.x, ansValue.y, 1.0f));
		break;
	case TransType::eScale:
		mOwner->GetTransform()->SetScale(Float3(ansValue.x, ansValue.y, 1.0f));
		break;
	}

	mCurrentTime += 1.0f;

	if (mCurrentTime < famly.totalTime)
		return;

	mEasingList.pop_front();
	isStart = false;
}

void Component::EasingImage::AddEasing(Easing::EasingType _easingType, TransType _transType, float _totalTime, DirectX::XMFLOAT2 _startValue, DirectX::XMFLOAT2 _endValue, bool _isStartAbsolute, bool _isEndAbsolute)
{
	EasingFamily newFamily;

	// 変数を構造体に入れる
	newFamily.easingType = _easingType;
	newFamily.transType = _transType;
	newFamily.totalTime = _totalTime;
	newFamily.startValue = _startValue;
	newFamily.endValue = _endValue;
	newFamily.isStartAbsolute = _isStartAbsolute;
	newFamily.isEndAbsolute = _isEndAbsolute;

	mEasingList.push_back(newFamily);	// リストに追加
}