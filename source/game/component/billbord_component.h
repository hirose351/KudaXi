#pragma once
#include	"component_base.h"
#include	"../../system/util/CBillboard.h"

namespace Component {
	class BillbordComponent : public ComponentBase
	{
	private:
		Dix::sp<Billboard> mpBillboard;

	public:
		void Init() override;
		void Update()override;
		void Draw()override;
		void ImguiDraw()override;
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