#include		"fade_screen.h"
#include		"../component/quad2d_component.h"
#include		"../../application.h"

#define			FADE_FRAME			(50.0f)

using namespace DirectX;

FadeScreen::FadeScreen() :GameObject(("FadeScreen"), ObjectType::eObstracle, false), mIsCompleted(true)
{
	mTransform->SetScale(Float3(static_cast<float>(Application::CLIENT_WIDTH), static_cast<float>(Application::CLIENT_HEIGHT), 0));
	mTransform->SetPositionXYZ(Float3(static_cast<float>(Application::CLIENT_WIDTH) / 2.0f, static_cast<float>(Application::CLIENT_HEIGHT) / 2.0f, 0));
	AddComponent<Component::Quad2d>()->SetInfo("", XMFLOAT4(0.9f, 0.8f, 0.8f, 1));
	GetComponent<Component::Quad2d>()->SetOrderInLayer(100);
	GetComponent<Component::Quad2d>()->SetDrawType(DrawType::eNoTex);
	mIsActive = false;
}

void FadeScreen::ObjectInit()
{
	mIsActive = false;
}

void FadeScreen::ObjectUpdate()
{
	if (!mIsActive)return;

	if (mCurrentType == FadeType::eIn)
	{
		if (mAlpha >= 1.0f)
		{
			mIsCompleted = true;
			mIsActive = false;
			SceneManager::GetInstance()->ChangeNextScene();
			return;
		}
		mAlpha += 1.0f / FADE_FRAME;
	}
	else if (mCurrentType == FadeType::eOut)
	{
		if (mAlpha <= 0.0f)
		{
			mIsCompleted = true;
			mIsActive = false;
			return;
		}
		mAlpha -= 1.0f / FADE_FRAME;
	}

	GetComponent<Component::Quad2d>()->SetColor(XMFLOAT4(0.9f, 0.8f, 0.8f, mAlpha));
}

void FadeScreen::SetFadeType(FadeType _type)
{
	mCurrentType = _type;
	mIsCompleted = false;
	mIsActive = true;
	if (mCurrentType == FadeType::eIn)
	{
		mAlpha = 0.0f;
	}
	else if (mCurrentType == FadeType::eOut)
	{
		mAlpha = 1.0f;
	}
}
