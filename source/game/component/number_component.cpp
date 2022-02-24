#include	"number_component.h"
#include	"quad2d_component.h"

using namespace Component;

int GetDigit(int num) {
	return static_cast<int> (std::to_string(num).length());
}

Number::Number() :ComponentBase(("Number"))
{
}

void Component::Number::Init()
{
	mQuad = mpOwner->GetComponents<Component::Quad2d>(GetComponentId());
}

void Number::Update()
{
	if (mQuad == nullptr)
		return;

	for (int i = 1; i <= GetDigit(mNum); i++)
	{
		// ‚PŒ…–Ú‚È‚ç
		if (i == 1)
			mQuad->SetDrawUv(INT2(mNum % (i * 10), 0));
		else
			mQuad->SetDrawUv(INT2((mNum + 1) / ((i - 1) * 10), 0));
		if (GetDigit(mNum) != 1)
			mQuad->SetDrawPos(Float3(mStartingPoint + mpOwner->GetTransform()->scale.x * (GetDigit(mNum) - i), mQuad->GetWorldPos().y, 0));
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

void Number::SetNum(int _num, float _space)
{
	mNum = _num;
	mSpace = _space;
	int digit = GetDigit(mNum);
	float scale = mpOwner->GetTransform()->scale.x;
	float pos = mQuad->GetWorldMtx()._41;
	mStartingPoint = pos - scale * (digit / 2) - (scale / 2.0f) * (digit % 2) - mSpace * (digit / 2) - mSpace / 2.0f * ((digit / 2) ? 1 : 0);
}