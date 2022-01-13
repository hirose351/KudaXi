#include	"billbord_component.h"

using namespace Component;

Billbord::Billbord() :ComponentBase(("Billbord")), mIsLateDraw(false)
{
	mpBillboard.SetPtr(new CBillboard);
}

void Billbord::Init()
{
}

void Billbord::Update()
{
	mpBillboard->Update(mOwner->GetTransform()->worldMtx);
}

void Billbord::Draw()
{
	if (mIsLateDraw) return;
	mpBillboard->Render();
}

void Billbord::LateDraw()
{
	if (!mIsLateDraw) return;
	mpBillboard->Render();
}

void Billbord::ImguiDraw()
{
}

void Billbord::Uninit()
{
}
