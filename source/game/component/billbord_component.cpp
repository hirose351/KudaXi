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
	mpBillboard->Update(mpOwner->GetTransform()->worldMtx);
}

void Billbord::Draw()
{
	mpBillboard->Render();
}

void Billbord::ImguiDraw()
{
}

void Billbord::Uninit()
{
}
