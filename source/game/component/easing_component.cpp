#include	"easing_component.h"
#include	"quad2d_component.h"

using namespace DirectX;

Component::Easing::Easing() :ComponentBase(("Easing"))
{
}

void Component::Easing::Update()
{
	if (mEasingList.empty())
		return;

	// Easingタイプを決める
	EasingFamily famly = mEasingList.front();

	if (!isStart)
	{
		if (famly.delayFrame > 0.0f)
		{
			famly.delayFrame--;
		}

		if (famly.isStartAbsolute)
		{
			switch (famly.transType)
			{
			case TransType::ePos:
				mEasingList.front().startValue = mOwner->GetTransform()->GetPosition();
				break;
			case TransType::eRot:
				mEasingList.front().startValue = mOwner->GetTransform()->GetAngle();
				break;
			case TransType::eScale:
				mEasingList.front().startValue = mOwner->GetTransform()->GetScale();
				break;
			}
		}
		if (famly.isEndAbsolute)
		{
			switch (famly.transType)
			{
			case TransType::ePos:
				mEasingList.front().endValue = mOwner->GetTransform()->GetPosition();
				break;
			case TransType::eRot:
				mEasingList.front().endValue = mOwner->GetTransform()->GetAngle();
				break;
			case TransType::eScale:
				mEasingList.front().endValue = mOwner->GetTransform()->GetScale();
				break;
			}
		}
		mCurrentFrame = 0;
		isStart = true;
	}

	// リストの先頭から値を求める
	famly = mEasingList.front();
	XMFLOAT2 ansValue;

	if (famly.startValue.x == famly.endValue.x)
		ansValue.x = famly.startValue.x;
	else
		ansValue.x = EasingProcess::GetEsingAns(famly.easingType, mCurrentFrame, famly.totalFrame, famly.startValue.x, famly.endValue.x);

	if (famly.startValue.y == famly.endValue.y)
		ansValue.y = famly.startValue.y;
	else
		ansValue.y = EasingProcess::GetEsingAns(famly.easingType, mCurrentFrame, famly.totalFrame, famly.startValue.y, famly.endValue.y);

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

	mCurrentFrame += 1.0f;

	if (mCurrentFrame < famly.totalFrame)
		return;

	mEasingList.pop_front();
	isStart = false;
}

void Component::Easing::AddEasing(EasingProcess::EasingType _easingType, TransType _transType, float _totalFrame, float _delayFrame, Float3 _startValue, Float3 _endValue, bool _isStartAbsolute, bool _isEndAbsolute)
{
	EasingFamily newFamily;

	// 変数を構造体に入れる
	newFamily.easingType = _easingType;
	newFamily.transType = _transType;
	newFamily.totalFrame = _totalFrame;
	newFamily.delayFrame = _delayFrame;
	newFamily.startValue = _startValue;
	newFamily.endValue = _endValue;
	newFamily.isStartAbsolute = _isStartAbsolute;
	newFamily.isEndAbsolute = _isEndAbsolute;

	mEasingList.push_back(newFamily);	// リストに追加
}