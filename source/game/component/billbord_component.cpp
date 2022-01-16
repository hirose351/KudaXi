#include	"billbord_component.h"

using namespace Component;

Billbord::Billbord() : mIsLateDraw(false)
{
	mpBillboard.SetPtr(new CBillboard);
	SetName("Billbord");
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

void Billbord::ImguiDraw()
{
}

void Billbord::Uninit()
{
}
