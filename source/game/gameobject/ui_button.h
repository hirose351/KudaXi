#pragma once
#include	"gameobject.h"
#include	<vector>

// �؂�ւ����@
enum class ButtonTransition
{
	eImageSwap,
	eColorTint,
};

// �{�^���̏��
enum class ButtonState
{
	eNomal = 0,		// �ʏ�(�I����)
	eSelected,		// �J�[�\���I��
	ePressed,		// �����ꂽ
	eDisabled,		// �I��s��
	MaxButtonState
};

// �`��J�n�n�_
enum class StartPoint
{
	eLeftUp,	// ����
	eRightUp,	// �E��
	eLeftDown,	// ����
	eRightDown, // �E��
};

/*

�I�����ꂽ�Ƃ��ɑ傫���Ȃ�
�g�����X�ɂ���ĉ摜���F���ς��
�摜���ς��Ȃ�{�^���̎�ނ��s�A�ς��摜����ɂ���




*/





namespace myUI {
	class Button : public GameObject
	{
	private:
		ButtonState mButtonState;		// �؂�ւ����@
		ButtonTransition mTransition;
		XMFLOAT2 mNomalScale;
		XMFLOAT2 mSelectScale;

	public:
		Button();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectDraw()override {};
		void ObjectImguiDraw()override {};
		void Uninit() override {};

		void SetTransition(ButtonTransition _trans)
		{
			mTransition = _trans;
		}
		void SetButtonState(ButtonState _state);
		void SetScale(XMFLOAT2 _nomalScale, XMFLOAT2 _selectScale)
		{
			mNomalScale = _nomalScale;
			mSelectScale = _selectScale;
		}
	};

	class ButtonGroup : public GameObject
	{
	private:
		std::vector<Button*> mButtonList;	// �{�^���̃��X�g
		StartPoint mStartPoint;				// �J�n�n�_
		DirectX::XMFLOAT2 mSpace;			// �]���̑傫��
		int mSelectNum;			// �I������Ă���ԍ�
		unsigned int mArrayCnt;				// ���ɕ��ׂ鐔
		unsigned int mPushTriggerFrame;		// ���������ɐi�ސ�
		XMFLOAT4 mStateColor[(int)ButtonState::MaxButtonState];

		void SetSelectedNum(int _num);

	public:
		ButtonGroup();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectDraw()override {};
		void ObjectImguiDraw()override;
		void Uninit() override {};

		// �����l�o�^(btCount:�T�C�R���̌�)
		void SetInitState(const char* _texName, int _divX, int _divY, int _arrayCnt, ButtonTransition _trans, XMFLOAT2 _startPos, XMFLOAT2 _nomalScale, XMFLOAT2 _selectScale, StartPoint _sP = StartPoint::eLeftUp);
		// �F�o�^
		void SetStateColor(ButtonState _state, const XMFLOAT4& _color)
		{
			mStateColor[(int)_state] = _color;
		}
	};
}