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
			Float3 startValue;								// �J�n���̒l
			Float3 endValue;								// �I�����̒l
			bool isStartAbsolute;							// �J�n���̒l�����Βl��
			bool isEndAbsolute;								// �I�����̒l�����Βl��
		};

		float mCurrentFrame;								// �o�ߎ���
		bool isStart = false;								// �J�n������
		std::list<EasingFamily> mEasingList;				// �C�[�W���O�̉򂪓��������X�g
		bool mIsRepeat = false;								// �J��Ԃ���

	public:
		Easing();
		void Init() override {};
		void Update()override;
		void ImguiDraw()override {};
		void Uninit() override {};

		// �C�[�W���O�ǉ�
		void AddEasing(EasingProcess::EasingType _easingType,
					   TransType _transType,
					   float _totalFrame,
					   float _delayFrame,
					   Float3 _startValue,
					   Float3 _endValue,
					   bool _isStartAbsolute = false,
					   bool _isEndAbsolute = false);

		// ���X�g�ɑ��݂���C�[�W���O�����̐���Ԃ�
		int GetEasingListCnt();

		void SetIsRepeat(bool _flg) { mIsRepeat = _flg; }
	};
}


