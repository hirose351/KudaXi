#include	"Player.h"
#include	"../component/player_controller.h"
#include	"../component/allcomponents.h"
#include	"../component/map_pos_component.h"
#include	"../manager/input_manager.h"
#include	"../../system/model/model_manager.h"
#include	<string>

Player::Player() : GameObject(("Player"), ObjectType::ePlayer, true) {
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/player/player.fbx",
		"shader/vs.hlsl", "shader/toonps.hlsl",
		"assets/model/player/");
	if (!sts)
	{
		MessageBox(nullptr, "Playerƒ‚ƒfƒ‹ “Ç‚Ýž‚ÝƒGƒ‰[", "error", MB_OK);
	}
	mTransform->scale = Float3(4, 6, 4);
	AddComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/player/player.fbx"));
	AddComponent<Component::PlayerController>();
	AddComponent<Component::MapPos>();
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