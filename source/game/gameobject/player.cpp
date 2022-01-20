#include	"Player.h"
#include	<string>
#include	"../component/player_controller.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"
#include	"../manager/input_manager.h"

Player::Player() : GameObject(("Player"), ObjectType::ePlayer, true) {
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/player/player.fbx",
		"shader/vs.hlsl", "shader/toonps.hlsl",
		"assets/model/player/");
	if (!sts)
	{
		MessageBox(nullptr, "Player���f�� �ǂݍ��݃G���[", "error", MB_OK);
	}
	mTransform->scale = Float3(4, 6, 4);
	AddComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/player/player.fbx"));
	AddComponent<Component::Collision>()->SetLocalScale(mTransform->scale);
	GetComponent<Component::Collision>()->SetColor(DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 0.01f));
	AddComponent<Component::PlayerController>();
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