#include	"quad2d_component.h"

using namespace Component;

Quad2d::Quad2d() : mDrawType(DrawType::eTex)
{
	mOrderInLayer = 100;
	SetName("Quad2d");
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
	TurnOffZbuffer();
	Transform transform;
	transform.SetWordMtx(mOwner->GetTransform()->worldMtx);

	// 連続描画用リストが空なら
	if (mDrawPosList.empty())
	{
		switch (mDrawType)
		{
		case DrawType::eTex:
			mpQuad->Draw(transform.GetMtx());
			break;
		case DrawType::eNoTex:
			mpQuad->DrawNoTex(transform.GetMtx());
			break;
		case DrawType::eMokomokoTex:
			mpQuad->DrawMokoMokoTex(transform.GetMtx());
			break;
		}
	}
	else
	{
		for (int i = 0; i < mDrawPosList.size(); i++)
		{
			transform.position = mDrawPosList[i];
			transform.CreateWordMtx();

			if (!mDrawUvList.empty())
				SetUvPos(mDrawUvList[i]);

			switch (mDrawType)
			{
			case DrawType::eTex:
				mpQuad->Draw(transform.GetMtx());
				break;
			case DrawType::eNoTex:
				mpQuad->DrawNoTex(transform.GetMtx());
				break;
			case DrawType::eMokomokoTex:
				mpQuad->DrawMokoMokoTex(transform.GetMtx());
				break;
			}
		}
		mDrawPosList.clear();
		mDrawUvList.clear();
	}

	TurnOnZbuffer();
}

void Quad2d::ImguiDraw()
{
	ImGui::DragInt("OrderInLayer", &mOrderInLayer, 0.5f);
}

void Quad2d::Uninit()
{
}

void Quad2d::SetInfo(std::string _texName, const XMFLOAT4& _color, int _u, int _v, float _z)
{
	mTexUv = { _u,_v };
	mpQuad.SetPtr(new CQuad2D);
	mColor = _color;
	mpQuad->Init(XMFLOAT2(mOwner->GetTransform()->GetScale().x, mOwner->GetTransform()->GetScale().y), _texName, _color, _u, _v, _z);
	mOwner->SetName(mOwner->GetName() + "  " + _texName);
}

void Quad2d::SetColor(const DirectX::XMFLOAT4 & _color)
{
	mColor = _color;
	Update();
}

void Quad2d::SetUvPos(INT2 _uv)
{
	mUvPos = _uv;
	mpQuad->SetTextureUV(_uv.x, _uv.z);
}

void Quad2d::SetDrawUv(INT2 _uv)
{
	mDrawUvList.emplace_back(_uv);
}