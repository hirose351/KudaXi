#include	"ui_button.h"
#include	"../component/quad2d_component.h"
#include	"../component/easing_image_component.h"
#include	"../../system/dx11/CDirectInput.h"

using namespace myUI;

Button::Button() :GameObject(("Button"), ObjectType::eObstracle, false)
{
	AddComponent<Component::Quad2d>();
	AddComponent<Component::EasingImage>();
	// 画像の登録は各ボタンでやる
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
			// イージング登録
			GetComponent<Component::EasingImage>()->AddEasing(Easing::EasingType::eLinear, TransType::eScale, 10.0f, XMFLOAT2(mTransform->GetScale().x, mTransform->GetScale().y), mNomalScale);



			break;
		case ButtonState::eSelected:
			// イージング登録
			GetComponent<Component::EasingImage>()->AddEasing(Easing::EasingType::eLinear, TransType::eScale, 10.0f, XMFLOAT2(mTransform->GetScale().x, mTransform->GetScale().y), mSelectScale);


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
	mPushTriggerFrame = 20;
	mSpace = XMFLOAT2(10, 10);
	mSelectNum = 0;
}

void ButtonGroup::ObjectInit()
{

}

void ButtonGroup::ObjectUpdate()
{
	// 長押しの処理
	static unsigned int pushTime = 0;	// 長押ししている時間
	// 左
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_A))
	{
		if ((mSelectNum - 1) >= 0)
			SetSelectedNum((mSelectNum - 1) % mArrayCnt);
		else
			SetSelectedNum(mArrayCnt - 1);
		pushTime = 0;
	}
	// 右
	else if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_D))
	{
		SetSelectedNum((mSelectNum + 1) % mArrayCnt);
		pushTime = 0;
	}
	// 上
	else if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_UP) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_W))
	{
		SetSelectedNum((mSelectNum + mArrayCnt) % mButtonList.size());
		pushTime = 0;
	}
	// 下
	else if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_S))
	{
		SetSelectedNum((mSelectNum - mArrayCnt) % mButtonList.size());
		pushTime = 0;
	}

	// 左
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
	{
		pushTime++;
		if (pushTime >= mPushTriggerFrame)
		{
			if ((mSelectNum - 1) >= 0)
				SetSelectedNum((mSelectNum - 1) % mArrayCnt);
			else
				SetSelectedNum(mArrayCnt - 1);
		}
	}
	// 右
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
	{
		pushTime++;
		if (pushTime >= mPushTriggerFrame)
			SetSelectedNum((mSelectNum + 1) % mArrayCnt);
	}
	// 上
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
	{
		pushTime++;
		if (pushTime >= mPushTriggerFrame)
			SetSelectedNum((mSelectNum + mArrayCnt) % mButtonList.size());
	}
	// 下
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
	{
		pushTime++;
		if (pushTime >= mPushTriggerFrame)
			SetSelectedNum((mSelectNum - mArrayCnt) % mButtonList.size());
	}

	if (pushTime >= mPushTriggerFrame)
		pushTime = 0;

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

void ButtonGroup::SetInitState(const char* _texName, int _divX, int _divY, int _arrayCnt, ButtonTransition _trans, XMFLOAT2 _startPos, XMFLOAT2 _nomalScale, XMFLOAT2 _selectScale, StartPoint _sP)
{
	mArrayCnt = _arrayCnt;
	for (int i = 0; i < _divX*_divY; i++)
	{
		// ボタン生成
		Button* b = new Button;

		// vector配列に追加
		mButtonList.emplace_back(b);

		b->GetComponent<Component::Quad2d>()->SetInfo(_nomalScale, _texName, XMFLOAT4(1, 1, 1, 1), _divX, _divY);
		b->GetComponent<Component::Quad2d>()->SetUV(i, 0);


		// 状態登録
		b->SetTransition(_trans);
		b->SetScale(_nomalScale, _selectScale);
		b->GetTransform()->SetScale(Float3(_nomalScale.x, _nomalScale.y, 0));



		b->SetButtonState(ButtonState::eNomal);
		b->GetComponent<Component::Quad2d>()->SetColor(mStateColor[(int)ButtonState::eNomal]);

		// 位置指定
		switch (_sP)
		{
		case StartPoint::eLeftUp:
			b->GetTransform()->SetPosition(Float3(_startPos.x + i * mSpace.x + i * _nomalScale.x, _startPos.y /*+ i * mSpace.x + i * _nomalScale.x*/, 0));
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

void ButtonGroup::SetSelectedNum(int _num)
{
	if (mSelectNum == _num)
		return;
	mButtonList[mSelectNum]->SetButtonState(ButtonState::eNomal);
	// 色変える
	mButtonList[mSelectNum]->GetComponent<Component::Quad2d>()->SetColor(mStateColor[(int)ButtonState::eNomal]);
	mButtonList[_num]->SetButtonState(ButtonState::eSelected);
	// 色変える
	mButtonList[_num]->GetComponent<Component::Quad2d>()->SetColor(mStateColor[(int)ButtonState::eSelected]);
	mSelectNum = _num;
}
