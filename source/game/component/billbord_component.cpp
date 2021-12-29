#include	"billbord_component.h"

using namespace Component;

Billbord::Billbord()
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
	mpBillboard->Render();
}

void Billbord::ImguiDraw()
{
}

void Billbord::Uninit()
{
}
