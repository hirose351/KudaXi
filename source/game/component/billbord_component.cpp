#include	"billbord_component.h"

using namespace Component;

BillbordComponent::BillbordComponent()
{
	mpBillboard.SetPtr(new Billboard);
}

void BillbordComponent::Init()
{
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
