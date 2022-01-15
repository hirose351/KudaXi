#include		"fade_screen.h"
#include		"../component/quad2d_component.h"
#include		"../../application.h"

#define			FADE_FRAME			(10.0f)

using namespace DirectX;

FadeScreen::FadeScreen() :GameObject(("FadeScreen"), ObjectType::eObstracle, false), mIsCompleted(false), mIsActive(false)
{
	mTransform->SetScale(Float3(static_cast<float>(Application::CLIENT_WIDTH), static_cast<float>(Application::CLIENT_HEIGHT), 0));
	mTransform->SetPositionXYZ(Float3(static_cast<float>(Application::CLIENT_WIDTH) / 2.0f, static_cast<float>(Application::CLIENT_HEIGHT) / 2.0f, 0));
	AddComponent<Component::Quad2d>()->SetInfo("assets/image/title/rogo.png", XMFLOAT4(1, 1, 1, 1));
}

void FadeScreen::ObjectInit()
{

}

void FadeScreen::ObjectUpdate()
{
	if (!mIsActive)return;

	if (mCurrentType == FadeType::eIn)
	{
		if (mAlha >= 1.0f)
		{
			mIsCompleted = true;
			mIsActive = false;
			return;
		}
		mAlha -= 1.0f / FADE_FRAME;
	}
	else if (mCurrentType == FadeType::eOut)
	{
		if (mAlha <= 0.0f)
		{
			mIsCompleted = true;
			mIsActive = false;
			return;
		}
		mAlha += 1.0f / FADE_FRAME;
	}

	GetComponent<Component::Quad2d>()->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, mAlha));
}

void FadeScreen::SetFadeType(FadeType _type)
{
	mCurrentType = _type;
	mIsCompleted = false;
	mIsActive = true;
	if (mCurrentType == FadeType::eIn)
	{
		mAlha = 1.0f;
	}
	else if (mCurrentType == FadeType::eOut)
	{
		mAlha = 0.0f;
	}
}
