#include	"ui_button.h"
#include	"../component/quad2d_component.h"
#include	"../component/easing_image_component.h"
#include	"../../system/dx11/CDirectInput.h"

using namespace myUI;

Button::Button() :GameObject(("Button"), ObjectType::eObstracle, false)
{
	AddComponent<Component::Quad2d>();
	AddComponent<Component::EasingImage>();
	// �摜�̓o�^�͊e�{�^���ł��
}

void Button::ObjectInit()
{
}

void Button::ObjectUpdate()
{
}

void Button::SetButtonState(ButtonState _state)
{
	switch (mTransition)
	{
	case ButtonTransition::eImageSwap:
		break;
	case ButtonTransition::eColorTint:
		switch (_state)
		{
		case ButtonState::eNomal:
			// �C�[�W���O�o�^
			GetComponent<Component::EasingImage>()->AddEasing(Easing::EasingType::eLinear, TransType::eScale, 10.0f, 0.0f, XMFLOAT2(mTransform->GetScale().x, mTransform->GetScale().y), mNomalScale);

			break;
		case ButtonState::eSelected:
			// �C�[�W���O�o�^
			GetComponent<Component::EasingImage>()->AddEasing(Easing::EasingType::eLinear, TransType::eScale, 10.0f, 0.0f, XMFLOAT2(mTransform->GetScale().x, mTransform->GetScale().y), mSelectScale);

			break;
		case ButtonState::ePressed:
			break;
		case ButtonState::eDisabled:
			break;
		default:
			break;
		}
		break;
	}
}

ButtonGroup::ButtonGroup() :GameObject(("ButtonGroup"), ObjectType::eObstracle, false)
{
	mPressingTriggerFrame = 20;
	mSelectNum = 0;
}

void ButtonGroup::ObjectInit()
{

}

void ButtonGroup::ObjectUpdate()
{
	// �������̏���
	static unsigned int pressFrame = 0;	// ���������Ă��鎞��

	// ��
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_A))
	{
		if ((mSelectNum - 1) >= 0)
			SetSelectedNum((mSelectNum - 1) % mArrayCnt);
		else
			SetSelectedNum(mArrayCnt - 1);
		pressFrame = 0;
	}
	// �E
	else if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_D))
	{
		SetSelectedNum((mSelectNum + 1) % mArrayCnt);
		pressFrame = 0;
	}
	// ��
	else if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_UP) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_W))
	{
		SetSelectedNum((mSelectNum + mArrayCnt) % mButtonList.size());
		pressFrame = 0;
	}
	// ��
	else if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_S))
	{
		SetSelectedNum((mSelectNum - mArrayCnt) % mButtonList.size());
		pressFrame = 0;
	}

	// ��
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
	{
		pressFrame++;
		if (pressFrame >= mPressingTriggerFrame)
		{
			if ((mSelectNum - 1) >= 0)
				SetSelectedNum((mSelectNum - 1) % mArrayCnt);
			else
				SetSelectedNum(mArrayCnt - 1);
		}
	}
	// �E
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
	{
		pressFrame++;
		if (pressFrame >= mPressingTriggerFrame)
			SetSelectedNum((mSelectNum + 1) % mArrayCnt);
	}
	// ��
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
	{
		pressFrame++;
		if (pressFrame >= mPressingTriggerFrame)
			SetSelectedNum((mSelectNum + mArrayCnt) % mButtonList.size());
	}
	// ��
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
	{
		pressFrame++;
		if (pressFrame >= mPressingTriggerFrame)
			SetSelectedNum((mSelectNum - mArrayCnt) % mButtonList.size());
	}

	if (pressFrame >= mPressingTriggerFrame)
		pressFrame = 0;

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RETURN))
	{
	}
}

void ButtonGroup::ObjectImguiDraw()
{
	if (ImGui::TreeNode("ButtonColor"))
	{
		ImGui::ColorButton("Nomal", ImVec4(mStateColor[0].x, mStateColor[0].y, mStateColor[0].z, mStateColor[0].w));
		ImGui::ColorButton("Selected", ImVec4(mStateColor[1].x, mStateColor[1].y, mStateColor[1].z, mStateColor[1].w));
		ImGui::ColorButton("Pressed", ImVec4(mStateColor[2].x, mStateColor[2].y, mStateColor[2].z, mStateColor[2].w));
		ImGui::ColorButton("Disabled", ImVec4(mStateColor[3].x, mStateColor[3].y, mStateColor[3].z, mStateColor[3].w));
		ImGui::TreePop();
	}
}

void ButtonGroup::SetInitState(const char* _texName, int _divX, int _divY, int _arrayCnt, ButtonTransition _trans, XMFLOAT2 _startPos, XMFLOAT2 _space, XMFLOAT2 _nomalScale, XMFLOAT2 _selectScale, ButtonArrangement _ar, StartPoint _sP)
{
	mArrayCnt = _arrayCnt;
	mSpace = _space;
	for (int i = 0; i < _divX*_divY; i++)
	{
		// �{�^������
		Button* b = new Button;

		// vector�z��ɒǉ�
		mButtonList.emplace_back(b);

		b->GetTransform()->SetScale(Float3(_nomalScale.x, _nomalScale.y, 0));
		b->GetComponent<Component::Quad2d>()->SetInfo(_texName, XMFLOAT4(1, 1, 1, 1), _divX, _divY);

		//�F�Ő؂�ւ���Ȃ�
		if (_trans == ButtonTransition::eColorTint)
			b->GetComponent<Component::Quad2d>()->SetUV(i, 0);
		// �摜�Ő؂�ւ���Ȃ�
		else if (_trans == ButtonTransition::eImageSwap)
		{

		}


		// ��ԓo�^
		b->SetTransition(_trans);
		b->SetScale(_nomalScale, _selectScale);
		b->GetTransform()->SetScale(Float3(_nomalScale.x, _nomalScale.y, 0));



		b->SetButtonState(ButtonState::eNomal);
		b->GetComponent<Component::Quad2d>()->SetColor(mStateColor[(int)ButtonState::eNomal]);

		// �ʒu�w��
		switch (_sP)
		{
		case StartPoint::eLeftUp:
			if (_ar == ButtonArrangement::eHorizontal)
				b->GetTransform()->SetPosition(Float3(_startPos.x + i * mSpace.x + i * _nomalScale.x, _startPos.y /*+ i * mSpace.x + i * _nomalScale.x*/, 0));
			if (_ar == ButtonArrangement::eVertical)
				b->GetTransform()->SetPosition(Float3(_startPos.x, _startPos.y + i * mSpace.x + i * _nomalScale.y, 0));
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
		b->GetTransform()->CreateMtx();

		// 

	}
}

void ButtonGroup::SetInitSelectNum(int _num)
{
	// �ύX��̃{�^���̏�ԂƐF��ς���
	mButtonList[_num]->SetButtonState(ButtonState::eSelected);
	mButtonList[_num]->GetComponent<Component::Quad2d>()->SetColor(mStateColor[(int)ButtonState::eSelected]);
	mSelectNum = _num;
}

void ButtonGroup::SetPosition(int _num, XMFLOAT2 _pos)
{
	mButtonList[_num]->GetTransform()->SetPosition(Float3(_pos.x, _pos.y, 0));
	mButtonList[_num]->GetTransform()->CreateMtx();
}

void ButtonGroup::SetSelectedNum(int _num)
{
	// ���݂̔ԍ��Ɠ����Ȃ�Ԃ�
	if (mSelectNum == _num)
		return;

	// ���݂̃{�^���ƕύX��̃{�^���̏�ԂƐF��ς���
	mButtonList[mSelectNum]->SetButtonState(ButtonState::eNomal);
	mButtonList[mSelectNum]->GetComponent<Component::Quad2d>()->SetColor(mStateColor[(int)ButtonState::eNomal]);
	mButtonList[_num]->SetButtonState(ButtonState::eSelected);
	mButtonList[_num]->GetComponent<Component::Quad2d>()->SetColor(mStateColor[(int)ButtonState::eSelected]);

	mSelectNum = _num;
}
