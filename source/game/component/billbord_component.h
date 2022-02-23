#pragma once
#include	"draw_component_base.h"
#include	"../../system/util/CBillboard.h"

namespace Component {
	class Billbord : public DrawComponentBase
	{
	private:
		Dix::sp<CBillboard> mpBillboard;

	public:
		Billbord();
		void Init() override;
		void Update()override;
		void Draw()override;
		void ImguiDrawComponent()override;
		void Uninit() override;

		void SetColor(const DirectX::XMFLOAT4& color)
		{
			mpBillboard->SetColor(color);
		}
		void SetScale(const DirectX::XMFLOAT2& scale)
		{
			mpBillboard->SetScale(scale);
		}
		void SetDivUV(const DirectX::XMFLOAT2& _divuv)
		{
			mpBillboard->SetDivUV(_divuv);
		}
		void SetUV(const DirectX::XMFLOAT2& _uv)
		{
			mpBillboard->SetUV(_uv);
		}
		void LoadTexture(const std::string pTexFileName)
		{
			mpBillboard->LoadTexture(pTexFileName);
		}
	};
}