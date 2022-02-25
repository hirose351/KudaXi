#include	"easing_component.h"
#include	"quad2d_component.h"
#include	"billbord_component.h"

using namespace DirectX;

Component::Easing::Easing() :ComponentBase(("Easing"))
{
}

void Component::Easing::Update()
{
	if (mEasingList.empty())
		return;

	// Easing�^�C�v�����߂�
	EasingFamily famly = mEasingList.front();

	if (!mIsStart)
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
				mEasingList.front().startValue = mpOwner->GetTransform()->GetPosition();
				break;
			case TransType::eRot:
				mEasingList.front().startValue = mpOwner->GetTransform()->GetAngle();
				break;
			case TransType::eScale:
				mEasingList.front().startValue = mpOwner->GetTransform()->GetScale();
				break;
			case TransType::eQuad2dAlha:
				mEasingList.front().startValue = mpOwner->GetComponents<Quad2d>(GetComponentId())->GetAlha();
				break;
			case TransType::eQuad2dLocalPos:
				mEasingList.front().startValue = mpOwner->GetComponents<Quad2d>(GetComponentId())->GetLocalPos();
				break;
			case TransType::eBilbordAlha:
				mpOwner->GetComponents<Billbord>(GetComponentId())->GetAlha();
				break;
			}
		}
		if (famly.isEndAbsolute)
		{
			switch (famly.transType)
			{
			case TransType::ePos:
				mEasingList.front().endValue = mpOwner->GetTransform()->GetPosition();
				break;
			case TransType::eRot:
				mEasingList.front().endValue = mpOwner->GetTransform()->GetAngle();
				break;
			case TransType::eScale:
				mEasingList.front().endValue = mpOwner->GetTransform()->GetScale();
				break;
			case TransType::eQuad2dAlha:
				mEasingList.front().endValue = mpOwner->GetComponents<Quad2d>(GetComponentId())->GetAlha();
				break;
			case TransType::eQuad2dLocalPos:
				mEasingList.front().endValue = mpOwner->GetComponents<Quad2d>(GetComponentId())->GetLocalPos();
				break;
			case TransType::eBilbordAlha:
				mpOwner->GetComponents<Billbord>(GetComponentId())->GetAlha();
				break;
			}
		}
		mCurrentFrame = 0;
		mIsStart = true;
	}

	// ���X�g�̐擪����l�����߂�
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
		mpOwner->GetTransform()->SetPositionXYZ(ansValue);
		break;
	case TransType::eRot:
		mpOwner->GetTransform()->SetAngle(ansValue);
		break;
	case TransType::eScale:
		mpOwner->GetTransform()->SetScale(ansValue);
		break;
	case TransType::eQuad2dAlha:
		mpOwner->GetComponents<Quad2d>(GetComponentId())->SetAlha(ansValue.x);
		break;
	case TransType::eQuad2dLocalPos:
		mpOwner->GetComponents<Quad2d>(GetComponentId())->SetLocalPos(ansValue);
		break;
	case TransType::eBilbordAlha:
		mpOwner->GetComponents<Billbord>(GetComponentId())->SetAlha(ansValue.x);
		break;
	}

	mCurrentFrame += 1.0f;

	if (mCurrentFrame < famly.totalFrame)
		return;


	switch (famly.transType)
	{
	case TransType::ePos:
		mpOwner->GetTransform()->SetPositionXYZ(famly.endValue);
		break;
	case TransType::eRot:
		mpOwner->GetTransform()->SetAngle(famly.endValue);
		break;
	case TransType::eScale:
		mpOwner->GetTransform()->SetScale(famly.endValue);
		break;
	case TransType::eQuad2dAlha:
		mpOwner->GetComponents<Quad2d>(GetComponentId())->SetAlha(famly.endValue.x);
		break;
	case TransType::eQuad2dLocalPos:
		mpOwner->GetComponents<Quad2d>(GetComponentId())->SetLocalPos(famly.endValue);
		break;
	case TransType::eBilbordAlha:
		mpOwner->GetComponents<Billbord>(GetComponentId())->SetAlha(famly.endValue.x);
		break;
	}

	// �J��Ԃ��Ȃ�擪�̗v�f�𖖔��ɓ���ď�����Ԃɖ߂��A�J��Ԃ��Ȃ��Ȃ烊�X�g�������

	mIsStart = false;
	if (!mIsRepeat)
	{
		mEasingList.pop_front();
	}
	else
	{
		EasingFamily frontEasing = mEasingList.front();
		mEasingList.pop_front();
		mEasingList.push_back(frontEasing);
	}
}

void Component::Easing::AddEasing(EasingProcess::EasingType _easingType, TransType _transType, float _totalFrame, float _delayFrame, Float3 _startValue, Float3 _endValue, bool _isStartAbsolute, bool _isEndAbsolute)
{
	EasingFamily newFamily;

	// �ϐ����\���̂ɓ����
	newFamily.easingType = _easingType;
	newFamily.transType = _transType;
	newFamily.totalFrame = _totalFrame;
	newFamily.delayFrame = _delayFrame;
	newFamily.startValue = _startValue;
	newFamily.endValue = _endValue;
	newFamily.isStartAbsolute = _isStartAbsolute;
	newFamily.isEndAbsolute = _isEndAbsolute;

	mEasingList.push_back(newFamily);	// ���X�g�ɒǉ�
}

int Component::Easing::GetEasingListCnt()
{
	return static_cast<int>(mEasingList.size());
}
