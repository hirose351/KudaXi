#include	"animation2d_component.h"

using namespace Component;

Animation2d::Animation2d() :ComponentBase(("Animation2d")), isRepeat(false), mFrameCnt(0), mCurrentFrame(0)
{
}

void Animation2d::Init()
{
	mCurrentFrame = 0;
}

void Animation2d::Update()
{
	if (mQuad == nullptr)
	{
		mQuad = mOwner->GetComponent<Quad2d>();
		if (mQuad == nullptr)
			return;
		mUvposU = mQuad->GetTexUv().x;
	}

	mQuad->SetUvPos(INT2(mCurrentFrame / mFrameCnt % mUvposU, mArray));
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
