#include	"ui_button.h"
#include	"../component/quad2d_component.h"
#include	"../../system/dx11/CDirectInput.h"

using namespace myUI;

Button::Button() :GameObject(("Button"), ObjectType::eObstracle, false)
{
	AddComponent<Component::Quad2d>();
	// �摜�̓o�^�͊e�{�^���ł��
}

void Button::ObjectInit()
{
}

void Button::ObjectUpdate()
{
}

void Button::SetTransition(ButtonTransition _trans)
{
	switch (_trans)
	{
	case ButtonTransition::eImageSwap:

		break;
	case ButtonTransition::eColorTint:

		break;
	default:
		break;
	}

}

void Button::SetButtonState(ButtonState _state)
{
	switch (mTransition)
	{
	case ButtonTransition::eImageSwap:

		break;
	case ButtonTransition::eColorTint:



		break;
	}
}

ButtonGroup::ButtonGroup() :GameObject(("ButtonGroup"), ObjectType::eObstracle, false)
{

}

void ButtonGroup::ObjectInit()
{

}

void ButtonGroup::ObjectUpdate()
{
	// ��
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_A))
	{
		SetSelectedNum((mSelectNum - 1) % mArrayCnt);
	}
	// �E
	else if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_D))
	{
		SetSelectedNum((mSelectNum + 1) % mArrayCnt);
	}
	// ��
	else if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_UP) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_W))
	{
		SetSelectedNum((mSelectNum + mArrayCnt) % mButtonList.size());
	}
	// ��
	else if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_S))
	{
		SetSelectedNum((mSelectNum - mArrayCnt) % mButtonList.size());
	}

	// �������̏���
	static unsigned int pushTime = 0;	// ���������Ă��鎞��
	// ��
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
	{
		if (pushTime >= mPushTriggerFrame)
			SetSelectedNum((mSelectNum - 1) % mArrayCnt);
	}
	// �E
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
	{
		if (pushTime >= mPushTriggerFrame)
			SetSelectedNum((mSelectNum + 1) % mArrayCnt);
	}
	// ��
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
	{
		if (pushTime >= mPushTriggerFrame)
			SetSelectedNum((mSelectNum + mArrayCnt) % mButtonList.size());
	}
	// ��
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
	{
		if (pushTime >= mPushTriggerFrame)
			SetSelectedNum((mSelectNum - mArrayCnt) % mButtonList.size());
	}

	if (pushTime >= mPushTriggerFrame)
		pushTime = 0;

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RETURN))
	{
		mTransform->ReSetValue();
	}
}

void ButtonGroup::ObjectImguiDraw()
{
}

void ButtonGroup::SetInitState(const char* _texName, int _divX, int _divY, ButtonTransition _trans, XMFLOAT2 _startPos, XMFLOAT2 _nomalScale, XMFLOAT2 _selectScale, StartPoint _sP)
{



	for (int i = 0; i < _divX*_divY; i++)
	{
		// �{�^������
		Button* b = new Button;


		b->GetComponent<Component::Quad2d>()->SetInfo(_nomalScale, _texName, XMFLOAT4(1, 1, 1, 1), _divX, _divY);

		// ��ԓo�^
		b->SetTransition(_trans);
		b->SetScale(_nomalScale, _selectScale);

		// �ʒu�w��
		switch (_sP)
		{
		case StartPoint::eLeftUp:
			b->GetTransform()->SetPosition(Float3(0, 0, 0));
			break;
		case StartPoint::eRightUp:
			b->GetTransform()->SetPosition(Float3(0, 0, 0));
			break;
		case StartPoint::eLeftDown:
			b->GetTransform()->SetPosition(Float3(0, 0, 0));
			break;
		case StartPoint::eRightDown:
			b->GetTransform()->SetPosition(Float3(0, 0, 0));
			break;
		default:
			break;
		}

		// 

	}
}

void ButtonGroup::SetSelectedNum(int _num)
{
	if (mSelectNum == _num)
		return;
	mButtonList[mSelectNum]->SetButtonState(ButtonState::eNomal);
	mButtonList[_num]->SetButtonState(ButtonState::eSelected);
	mSelectNum = _num;
}
