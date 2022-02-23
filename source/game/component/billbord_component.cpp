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
	mpBillboard->Render();
}

void Billbord::ImguiDrawComponent()
{
}

void Billbord::Uninit()
{
}
