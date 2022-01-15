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
			float totalFrame = 0.0f;						// �K�v�Ȏ���
			float delayFrame = 0.0f;						// �x������
			DirectX::XMFLOAT2 startValue = { 0.0f,0.0f };	// �͂��߂̐�
			DirectX::XMFLOAT2 endValue = { 0.0f,0.0f };		// �I���̐�
			bool isStartAbsolute;
			bool isEndAbsolute;
		};

		float mCurrentFrame;									// �o�ߎ���
		bool isStart = false;								// �J�n������
		std::list<EasingFamily> mEasingList;				// �C�[�W���O�̉򂪓��������X�g
		DirectX::XMFLOAT2 mStartValue = { 0, 0 };					// ���݂̏����̂͂��߂̐�

	public:
		EasingImage();
		void Init() override {};
		void Update()override;
		void Draw()override {};
		void ImguiDraw()override {};
		void Uninit() override {};

		// startValue��0,0�Ȃ瑊�΍��W
		void AddEasing(Easing::EasingType _easingType,
					   TransType _transType,
					   float _totalFrame,
					   float _delayFrame,
					   DirectX::XMFLOAT2 _startValue,
					   DirectX::XMFLOAT2 _endValue,
					   bool _isStartAbsolute = false,
					   bool _isEndAbsolute = false);
	};
}


