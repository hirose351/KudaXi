#include	"quad2d_component.h"
#include	"../../system/util/quad2d.h"

using namespace Component;

void Quad2dComponent::Init()
{
}

void Quad2dComponent::Update()
{
}

void Quad2dComponent::Draw()
{
	mpQuad->Draw(mOwner->GetTransform()->worldMtx);
}

void Quad2dComponent::ImguiDraw()
{
}

void Quad2dComponent::Uninit()
{
}

void Quad2dComponent::SetInfo(int width, int height, const char * tex_name, const DirectX::XMFLOAT4 & color, int _u, int _v, float z)
{
	mpQuad->Init(width, height, tex_name, color, _u, _v, z);
}
