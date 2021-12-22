#include	"Player.h"
#include	<string>

Player::~Player()
{
	Uninit();
};

void Player::ObjectInit()
{
	mTransform->ReSetValue();
}

void Player::ObjectUpdate()
{
}

void Player::ObjectImguiDraw()
{
}

void Player::Uninit()
{

}

void Player::OnColEnterObj(Dice* _other)
{
	//// 最も近いサイコロを検索
	//SetNearestDice();

	//if (mPstate != eMove || mFoot != Foot::eFloor)
	//	return;
	//if (mTransform->GetPosition().y > DICE_SCALE_HALF)
	//	return;
	//if (!_other->SetPushAction(mDirection))
	//	return;
	//mpOperationDice = _other;
	//mPstate = ePush;	// 状態を変える
}

void Player::OnColStayObj(Dice* _other)
{
	//// 最も近いサイコロを検索
	//SetNearestDice();
}

void Player::OnColExitObj(Dice* _other)
{
	//if (mpOperationDice == _other && DICE_SCALE_HALF > mTransform->position.y&&mPstate != ePush)
	//{
	//	mpOperationDice = nullptr;
	//}
}

void Player::OnCollisionEnter(ComponentBase* _oher)
{
	//std::cout << "OnCollisionEnter　ObjectName:" + _oher->GetOwner()->GetName() + "\n";

	//if ((_oher->GetTag() == ObjectTag::eDice || _oher->GetTag() == ObjectTag::eDiceTop))
	//{
	//	OnColEnterObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	//}
}

void Player::OnCollisionStay(ComponentBase* _oher)
{
	//std::cout << "OnCollisionStay　ObjectName:" + _oher->GetOwner()->GetName() + "\n";
	//if (_oher->GetTag() == ObjectTag::eDice || _oher->GetTag() == ObjectTag::eDiceTop)
	//{
	//	OnColStayObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	//}
}

void Player::OnCollisionExit(ComponentBase* _oher)
{
	//std::cout << "OnCollisionExit　ObjectName:" + _oher->GetOwner()->GetName() + "\n";
	//if ((_oher->GetTag() == ObjectTag::eDice || _oher->GetTag() == ObjectTag::eDiceTop))
	//{
	//	OnColExitObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	//}
}