#include	"quad2d_component.h"

using namespace Component;

Quad2d::Quad2d() :ComponentBase(("Quad2d")), mIsLateDraw(false), mDrawType(DrawType::eTex)
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
	if (mIsLateDraw) return;
	TurnOffZbuffer();

	switch (mDrawType)
	{
	case DrawType::eTex:
		mpQuad->Draw(mOwner->GetTransform()->worldMtx);
		break;
	case DrawType::eNoTex:
		mpQuad->DrawNoTex();
		break;
	case DrawType::eMokomokoTex:
		mpQuad->DrawMokoMokoTex(mOwner->GetTransform()->worldMtx);
		break;
	default:
		break;
	}

	TurnOnZbuffer();
}

void Quad2d::LateDraw()
{
	if (!mIsLateDraw) return;
	TurnOffZbuffer();
	mpQuad->Draw(mOwner->GetTransform()->worldMtx);
	TurnOnZbuffer();
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

void Quad2d::SetUV(int u, int v)
{
	mpQuad->SetTextureUV(u, v);
}