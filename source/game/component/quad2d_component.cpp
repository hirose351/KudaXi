#include	"quad2d_component.h"
#include	"../../system/util/quad2d.h"

using namespace Component;

void Quad2d::Init()
{
}

void Quad2d::Update()
{
	mpQuad->UpdateVertex(XMFLOAT2(mOwner->GetTransform()->GetScale().x, mOwner->GetTransform()->GetScale().y), XMFLOAT4(1, 1, 1, 1));
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

void Quad2d::SetInfo(XMFLOAT2 _scale, const char* tex_name, const XMFLOAT4& color, int _u, int _v, float z)
{
	mpQuad.SetPtr(new CQuad2D);
	mpQuad->Init(_scale, tex_name, color, _u, _v, z);
}

void Quad2d::SetScale(XMFLOAT2 _scale)
{
	mpQuad->UpdateVertex(_scale, XMFLOAT4(1, 1, 1, 1));
	mpQuad->UpdateVbuffer();
}
