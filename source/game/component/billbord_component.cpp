#include	"billbord_component.h"

using namespace Component;

void BillbordComponent::Init()
{
	mpBillboard.SetPtr(new Billboard);
}

void BillbordComponent::Update()
{
	mpBillboard->Update(mOwner->GetTransform()->worldMtx);
}

void BillbordComponent::Draw()
{
	mpBillboard->Render();
}

void BillbordComponent::ImguiDraw()
{
}

void BillbordComponent::Uninit()
{
}
