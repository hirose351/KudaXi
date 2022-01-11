#include	"Player.h"
#include	<string>
#include	"../component/player_controller.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"

Player::Player() : GameObject(("Player"), ObjectType::ePlayer, true) {
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/player/player.fbx",
		"shader/vs.hlsl", "shader/toonps.hlsl",
		"assets/model/player/");
	if (!sts)
	{
		MessageBox(nullptr, "Playerモデル 読み込みエラー", "error", MB_OK);
	}
	mTransform->scale = Float3(4, 6, 4);
	AddComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/player/player.fbx"));
	AddComponent<Component::Collision>()->SetLocalScale(mTransform->scale);
	GetComponent<Component::Collision>()->SetColor(DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 0.01f));
	AddComponent<Component::PlayerController>();
	ObjectInit();
};

Player::~Player()
{
	Uninit();
};

void Player::ObjectInit()
{
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