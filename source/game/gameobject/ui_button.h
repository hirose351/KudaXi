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

// ���ו�
enum class ButtonArrangement
{
	eHorizontal,	// ��
	eVertical,		// �c
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

�g�p���̓o�^��
�F��������񁨏����I��ԍ�

*/

namespace myUI {
	class Button : public GameObject
	{
	private:
		ButtonState mButtonState;		// �؂�ւ����@
		ButtonTransition mTransition;
		DirectX::XMFLOAT2 mNomalScale;
		DirectX::XMFLOAT2 mSelectScale;

	public:
		Button();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectImguiDraw()override {};
		void Uninit() override {};

		void SetTransition(ButtonTransition _trans)
		{
			mTransition = _trans;
		}
		void SetButtonState(ButtonState _state);
		void SetScale(DirectX::XMFLOAT2 _nomalScale, DirectX::XMFLOAT2 _selectScale)
		{
			mNomalScale = _nomalScale;
			mSelectScale = _selectScale;
		}
	};

	class ButtonGroup : public GameObject
	{
	private:
		std::vector<Button*> mButtonList;		// �{�^���̃��X�g
		StartPoint mStartPoint;					// �J�n�n�_
		DirectX::XMFLOAT2 mSpace;				// �]���̑傫��
		int mSelectNum;							// �I������Ă���ԍ�
		unsigned int mArrayCnt;					// ���ɕ��ׂ鐔
		unsigned int mPressingTriggerFrame;		// ���������ɐi�ސ�
		DirectX::XMFLOAT4 mStateColor[(int)ButtonState::MaxButtonState] = { DirectX::XMFLOAT4(0.5f,0.5f,0.5f,1.0f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f) };
		bool isPressed = false;		// �{�^���������ꂽ��
		ButtonArrangement mArrangement;
		DirectX::XMFLOAT2 mNomalScale;
		Float3 mBeforePos;

		// �I�����ꂽ���̏���
		void SetSelectedNum(int _num);

		void SetButtonPosition();
	public:
		ButtonGroup();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectImguiDraw()override;
		void Uninit() override {};

		// �F�o�^
		void SetStateColor(ButtonState _state, const DirectX::XMFLOAT4& _color)
		{
			mStateColor[(int)_state] = _color;
		}

		// �����l�o�^(btCount:�T�C�R���̌�)
		void SetInitState(const char* _texName, int _divX, int _divY, int _arrayCnt, ButtonTransition _trans, DirectX::XMFLOAT2 _space, DirectX::XMFLOAT2 _nomalScale, DirectX::XMFLOAT2 _selectScale, ButtonArrangement _ar = ButtonArrangement::eHorizontal, StartPoint _sP = StartPoint::eLeftUp);

		// �����I��ԍ��o�^
		void SetInitSelectNum(int _num);

		// 
		void SetPosition(int _num, DirectX::XMFLOAT2 _pos);

		// �I������Ă���ԍ���Ԃ�
		int GetSelectNum() { return mSelectNum; }

		// �����ꂽ����Ԃ�
		bool GetIsPressed() { return isPressed; }
	};
}