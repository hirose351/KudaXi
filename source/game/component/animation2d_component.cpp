#include	"animation2d_component.h"

using namespace Component;

Animation2d::Animation2d() :ComponentBase(("Animation2d")), mIsRepeat(false), mFrameCnt(0), mCurrentFrame(0)
{
}

void Animation2d::Init()
{
	mCurrentFrame = 0;
}

void Animation2d::Update()
{
	if (mpQuad == nullptr)
	{
		mpQuad = mpOwner->GetComponent<Quad2d>();
		if (mpQuad == nullptr)
			return;
		mUvposU = mpQuad->GetTexUv().x;
	}

	mpQuad->SetUvPos(INT2(mCurrentFrame / mFrameCnt % mUvposU, mArray));
	mCurrentFrame++;
	if (mCurrentFrame / mFrameCnt >= mUvposU)
		mCurrentFrame = 0;
}

void Animation2d::ImguiDraw()
{

}

void Animation2d::SetAnim(int _speed, int array)
{
	mFrameCnt = _speed;
	mArray = array;
}
