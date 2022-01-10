#include	"quad2d_component.h"
#include	"../../system/util/quad2d.h"

using namespace Component;

Quad2d::Quad2d() :ComponentBase(("Quad2d"))
{
}

void Quad2d::Init()
{
}

void Quad2d::Update()
{
	mpQuad->UpdateVertex(XMFLOAT2(mOwner->GetTransform()->GetScale().x, mOwner->GetTransform()->GetScale().y), mColor);
	mpQuad->UpdateVbuffer();
}

void Quad2d::Draw()
{
	mpQuad->Draw(mOwner->GetTransform()->worldMtx);
}

void Quad2d::ImguiDraw()
{
}

void Quad2d::Uninit()
{
}

void Quad2d::SetInfo(const char* _texName, const XMFLOAT4& _color, int _u, int _v, float _z)
{
	mpQuad.SetPtr(new CQuad2D);
	mpQuad->Init(XMFLOAT2(mOwner->GetTransform()->GetScale().x, mOwner->GetTransform()->GetScale().y), _texName, _color, _u, _v, _z);
}
