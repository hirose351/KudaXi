#pragma once
#include	"component_base.h"
#include	"../../system/util/easing.h"
#include	<list>

// ����������
enum class TransType
{
	ePos,
	eRot,
	eScale
};

namespace Component {
	class EasingImage : public ComponentBase
	{
	private:
		// �C�[�W���O�ɕK�v�Ȍł܂�
		struct EasingFamily
		{
			Easing::EasingType easingType = Easing::EasingType::eLinear;
			TransType transType = TransType::ePos;
			float totalTime = 0.0f;							// �K�v�Ȏ���
			DirectX::XMFLOAT2 startValue = { 0.0f,0.0f };	// �͂��߂̐�
			DirectX::XMFLOAT2 endValue = { 0.0f,0.0f };		// �I���̐�
		};

		float mTotalTime;	 // 
		float mCurrentTime;  // �o�ߎ���
		float mEndValue;	 // 

		bool isStart = false;

		std::list<EasingFamily> mEasingList;
		XMFLOAT2 mStartValue = { 0, 0 };

	public:
		void Init() override {};
		void Update()override;
		void Draw()override {};
		void ImguiDraw()override {};
		void Uninit() override {};

		// startValue��0,0�Ȃ瑊�΍��W
		void AddEasing(Easing::EasingType _easingType,
					   TransType _transType,
					   float _totalTime,
					   DirectX::XMFLOAT2 _startValue,
					   DirectX::XMFLOAT2 _endValue);
	};
}


