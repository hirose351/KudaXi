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
	class Easing : public ComponentBase
	{
	private:
		// �C�[�W���O�ɕK�v�Ȍł܂�
		struct EasingFamily
		{
			EasingProcess::EasingType easingType = EasingProcess::EasingType::eLinear;
			TransType transType = TransType::ePos;
			float totalFrame = 0.0f;						// �K�v�Ȏ���
			float delayFrame = 0.0f;						// �x������
			Float3 startValue;	// �͂��߂̐�
			Float3 endValue;		// �I���̐�
			bool isStartAbsolute;
			bool isEndAbsolute;
		};

		float mCurrentFrame;									// �o�ߎ���
		bool isStart = false;								// �J�n������
		std::list<EasingFamily> mEasingList;				// �C�[�W���O�̉򂪓��������X�g
		DirectX::XMFLOAT2 mStartValue = { 0, 0 };					// ���݂̏����̂͂��߂̐�

	public:
		Easing();
		void Init() override {};
		void Update()override;
		void ImguiDraw()override {};
		void Uninit() override {};

		// startValue��0,0�Ȃ瑊�΍��W
		void AddEasing(EasingProcess::EasingType _easingType,
					   TransType _transType,
					   float _totalFrame,
					   float _delayFrame,
					   Float3 _startValue,
					   Float3 _endValue,
					   bool _isStartAbsolute = false,
					   bool _isEndAbsolute = false);
	};
}


