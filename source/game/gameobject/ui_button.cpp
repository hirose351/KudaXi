#include	"ui_button.h"
#include	"../component/quad2d_component.h"
#include	"../component/easing_component.h"
#include	"../manager/input_manager.h"

using namespace myUI;
using namespace DirectX;

Button::Button() :GameObject(("Button"), ObjectType::eObstracle, false)
{
	AddComponent<Component::Quad2d>();
	AddComponent<Component::Easing>();
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
			GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::eScale, 10.0f, 0.0f, XMFLOAT2(mTransform->GetScale().x, mTransform->GetScale().y), mNomalScale);

			break;
		case ButtonState::eSelected:
			// イージング登録
			GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::eScale, 10.0f, 0.0f, XMFLOAT2(mTransform->GetScale().x, mTransform->GetScale().y), mSelectScale);

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
	isPressed = false;		// ボタンが押されたか
}

void ButtonGroup::ObjectUpdate()
{
	static unsigned int pressFrame = 0;	// 長押ししている時間

	// 押された瞬間の処理
	InputDirection i = InputManager::GetInstance().GetDirectionTrigger(InputMode::eUi, static_cast<int>(UiAction::eNavigate));
	switch (i)
	{
		// 左
	case InputDirection::eLeft:
	{
		if ((mSelectNum - 1) >= 0)
			SetSelectedNum((mSelectNum - 1) % mArrayCnt);
		else
			SetSelectedNum(mArrayCnt - 1);
		pressFrame = 0;
		break;
	}
	// 右
	case InputDirection::eRight:
	{
		SetSelectedNum((mSelectNum + 1) % mArrayCnt);
		pressFrame = 0;
		break;
	}
	// 上
	case InputDirection::eUp:
	{
		SetSelectedNum((mSelectNum + mArrayCnt) % mpButtonList.size());
		pressFrame = 0;
		break;
	}
	// 下
	case InputDirection::eDown:
	{
		SetSelectedNum((mSelectNum - mArrayCnt) % mpButtonList.size());
		pressFrame = 0;
		break;
	}
	default:
		break;
	}

	// 長押しの処理
	i = InputManager::GetInstance().GetDirection(InputMode::eUi, static_cast<int>(UiAction::eNavigate));
	switch (i)
	{
		// 左
	case InputDirection::eLeft:
	{
		pressFrame++;
		if (pressFrame >= mPressingTriggerFrame)
		{
			if ((mSelectNum - 1) >= 0)
				SetSelectedNum((mSelectNum - 1) % mArrayCnt);
			else
				SetSelectedNum(mArrayCnt - 1);
		}
		break;
	}
	// 右
	case InputDirection::eRight:
	{
		pressFrame++;
		if (pressFrame >= mPressingTriggerFrame)
			SetSelectedNum((mSelectNum + 1) % mArrayCnt);
		break;
	}
	// 上
	case InputDirection::eUp:
	{
		pressFrame++;
		if (pressFrame >= mPressingTriggerFrame)
			SetSelectedNum((mSelectNum + mArrayCnt) % mpButtonList.size());
		break;
	}
	// 下
	case InputDirection::eDown:
	{
		pressFrame++;
		if (pressFrame >= mPressingTriggerFrame)
			SetSelectedNum((mSelectNum - mArrayCnt) % mpButtonList.size());
		break;
	}
	default:
		break;
	}

	if (pressFrame >= mPressingTriggerFrame)
		pressFrame = 0;

	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eClick)))
	{
		isPressed = true;
	}

	//if (mParent != nullptr)
	//{
	//	mTransform->position = mParent->GetTransform()->position;

	//}

	// 位置が変わっていたら全てのボタンの位置を変更
	if (mBeforePos != mTransform->position)
		SetButtonPosition();

	mBeforePos = mTransform->position;
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

void ButtonGroup::SetInitState(const char* _texName, int _divX, int _divY, int _arrayCnt, ButtonTransition _trans, XMFLOAT2 _space, XMFLOAT2 _nomalScale, XMFLOAT2 _selectScale, ButtonArrangement _ar, StartPoint _sP)
{
	mArrayCnt = _arrayCnt;
	mSpace = _space;
	mStartPoint = _sP;
	mNomalScale = _nomalScale;
	mBeforePos = mTransform->position;
	mArrangement = _ar;

	for (int i = 0; i < _divX*_divY; i++)
	{
		// ボタン生成
		Dix::sp<Button> b;
		b.SetPtr(new Button);
		SceneManager::GetInstance()->GetScene(mSceneKey)->AddGameObject(b);

		b->SetParent(this);

		b->GetTransform()->SetScale(Float3(_nomalScale.x, _nomalScale.y, 0));
		b->GetComponent<Component::Quad2d>()->SetInfo(_texName, XMFLOAT4(1, 1, 1, 1), _divX, _divY);

		//色で切り替えるなら
		if (_trans == ButtonTransition::eColorTint)
			b->GetComponent<Component::Quad2d>()->SetUV(i, 0);
		// 画像で切り替えるなら
		else if (_trans == ButtonTransition::eImageSwap)
		{

		}


		// 状態登録
		b->SetTransition(_trans);
		b->SetScale(_nomalScale, _selectScale);
		b->GetTransform()->SetScale(Float3(_nomalScale.x, _nomalScale.y, 0));

		b->SetButtonState(ButtonState::eNomal);
		b->GetComponent<Component::Quad2d>()->SetColor(mStateColor[(int)ButtonState::eNomal]);

		// 位置指定
		SetButtonPosition();

		// vector配列に追加
		mpButtonList.emplace_back(b);
	}
}

void ButtonGroup::SetInitSelectNum(int _num)
{
	// 変更後のボタンの状態と色を変える
	mpButtonList[_num]->SetButtonState(ButtonState::eSelected);
	mpButtonList[_num]->GetComponent<Component::Quad2d>()->SetColor(mStateColor[(int)ButtonState::eSelected]);
	mSelectNum = _num;
}

void ButtonGroup::SetPosition(int _num, XMFLOAT2 _pos)
{
	mpButtonList[_num]->GetTransform()->SetPosition(Float3(_pos.x, _pos.y, 0));
	mpButtonList[_num]->GetTransform()->CreateWordMtx();
}

void ButtonGroup::SetSelectedNum(int _num)
{
	// 現在の番号と同じなら返す
	if (mSelectNum == _num)
		return;

	// 現在のボタンと変更後のボタンの状態と色を変える
	mpButtonList[mSelectNum]->SetButtonState(ButtonState::eNomal);
	mpButtonList[mSelectNum]->GetComponent<Component::Quad2d>()->SetColor(mStateColor[(int)ButtonState::eNomal]);
	mpButtonList[_num]->SetButtonState(ButtonState::eSelected);
	mpButtonList[_num]->GetComponent<Component::Quad2d>()->SetColor(mStateColor[(int)ButtonState::eSelected]);

	mSelectNum = _num;
}

void ButtonGroup::SetButtonPosition()
{
	int i = 0;
	for (Dix::wp<Button> b : mpButtonList)
	{
		// 位置指定
		switch (mStartPoint)
		{
		case StartPoint::eLeftUp:
			if (mArrangement == ButtonArrangement::eHorizontal)
				//b->SetStartPos(Float3(mTransform->position.x + i * mSpace.x + i * mNomalScale.x, mTransform->position.y /*+ i * mSpace.x + i * _nomalScale.x*/, 0));
				b->GetTransform()->SetPosition(Float3(mTransform->position.x + i * mSpace.x + i * mNomalScale.x, mTransform->position.y /*+ i * mSpace.x + i * _nomalScale.x*/, 0));
			if (mArrangement == ButtonArrangement::eVertical)
				//b->SetStartPos(Float3(mTransform->position.x, mTransform->position.y + i * mSpace.y + i * mNomalScale.y, 0));
				b->GetTransform()->SetPosition(Float3(mTransform->position.x, mTransform->position.y + i * mSpace.y + i * mNomalScale.y, 0));
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
		b->GetTransform()->CreateWordMtx();

		i++;
	}
}