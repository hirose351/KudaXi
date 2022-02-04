#include	"easing_component.h"

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
	Float3 ansValue;

	if (famly.startValue.x == famly.endValue.x)
		ansValue.x = famly.startValue.x;
	else
		ansValue.x = EasingProcess::GetEsingAns(famly.easingType, mCurrentFrame, famly.totalFrame, famly.startValue.x, famly.endValue.x);

	if (famly.startValue.y == famly.endValue.y)
		ansValue.y = famly.startValue.y;
	else
		ansValue.y = EasingProcess::GetEsingAns(famly.easingType, mCurrentFrame, famly.totalFrame, famly.startValue.y, famly.endValue.y);

	if (famly.startValue.z == famly.endValue.z)
		ansValue.z = famly.startValue.z;
	else
		ansValue.z = EasingProcess::GetEsingAns(famly.easingType, mCurrentFrame, famly.totalFrame, famly.startValue.z, famly.endValue.z);

	switch (famly.transType)
	{
	case TransType::ePos:
		mOwner->GetTransform()->SetPositionXYZ(ansValue);
		break;
	case TransType::eRot:
		mOwner->GetTransform()->SetAngle(ansValue);
		break;
	case TransType::eScale:
		mOwner->GetTransform()->SetScale(ansValue);
		break;
	}

	mCurrentFrame += 1.0f;

	if (mCurrentFrame < famly.totalFrame)
		return;

	mEasingList.pop_front();
	isStart = false;

	switch (famly.transType)
	{
	case TransType::ePos:
		mOwner->GetTransform()->SetPositionXYZ(famly.endValue);
		break;
	case TransType::eRot:
		mOwner->GetTransform()->SetAngle(famly.endValue);
		break;
	case TransType::eScale:
		mOwner->GetTransform()->SetScale(famly.endValue);
		break;
	}
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

int Component::Easing::GetEasingListCnt()
{
	return mEasingList.size();
}
