#include	"billbord_component.h"

using namespace Component;

Billbord::Billbord()
{
	mpBillboard.SetPtr(new CBillboard);
	SetName("Billbord");
}

void Billbord::Init()
{
}

void Billbord::Update()
{
	mpBillboard->Update(mWorldMtx);
}

void Billbord::Draw()
{
	// 連続描画用リストが空なら
	if (mDrawPosList.empty())
	{
		mpBillboard->Render();
	}
	else
	{
		for (int i = 0; i < mDrawPosList.size(); i++)
		{
			mWorldMtx._41 = mDrawPosList[i].x;
			mWorldMtx._42 = mDrawPosList[i].y;
			mWorldMtx._43 = mDrawPosList[i].z;
			Update();
			mpBillboard->Render();
		}
		mDrawPosList.clear();
	}
}

void Billbord::ImguiDrawComponent()
{
}

void Billbord::Uninit()
{
}
