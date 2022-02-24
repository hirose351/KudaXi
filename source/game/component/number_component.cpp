#include	"number_component.h"
#include	"quad2d_component.h"
#include	<cmath>

using namespace Component;

int GetDigit(int num) {
	return static_cast<int> (std::to_string(num).length());
}

Number::Number() :ComponentBase(("Number"))
{
}

void Number::Init()
{
	mQuad = mpOwner->GetComponents<Component::Quad2d>(GetComponentId());
}

void Number::Update()
{
	if (mQuad == nullptr)
		return;
	int digit = mIsDigitFixed ? mDigit : GetDigit(mNum);
	for (int i = 1; i <= digit; i++)
	{
		// uvÝ’è
		mQuad->SetDrawUv(INT2((mNum  % static_cast<int>(std::pow(10, i))) / static_cast<int>(std::pow(10, i - 1)), 0));

		// ˆÊ’uÝ’è
		if (digit != 1)
			mQuad->SetDrawPos(Float3(mStartingPoint + mpOwner->GetTransform()->scale.x * (digit - i), mQuad->GetWorldPos().y, 0));
		else
			mQuad->SetDrawPos(mQuad->GetWorldPos());
	}
}

void Number::ImguiDraw()
{
	ImGui::DragInt("number", &mNum, 0.5f);
	ImGui::DragFloat("space", &mSpace, 0.5f);
	SetNum(mNum);
}

void Number::SetNum(int _num, float _space, int _digit)
{
	mNum = _num;
	mSpace = _space;
	mDigit = _digit < GetDigit(mNum) ? GetDigit(mNum) : _digit;
	mIsDigitFixed = _digit < GetDigit(mNum) ? false : true;
	float scale = mpOwner->GetTransform()->scale.x;
	float pos = mQuad->GetWorldMtx()._41;
	mStartingPoint = pos - scale * (mDigit / 2) - (scale / 2.0f) * (mDigit % 2) - mSpace * (mDigit / 2) - mSpace / 2.0f * ((mDigit / 2) ? 1 : 0);
}