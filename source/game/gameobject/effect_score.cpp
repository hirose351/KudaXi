#include	"effect_score.h"
#include	"../component/quad2d_component.h"
#include	"../component/number_component.h"
#include	"../component/easing_component.h"

using namespace Effect;

Score::Score() :GameObject(("ScoreEffect"), ObjectType::eObstracle, false)
{
	mTransform->scale = 30;
	AddComponent<Component::Quad2d>()->SetInfo("assets/image/ui/number.png", XMFLOAT4(1, 1, 1, 1), 10);
	AddComponent<Component::Quad2d>()->SetInfo("assets/image/ui/number.png", XMFLOAT4(1, 1, 1, 1), 10);
	GetComponent<Component::Quad2d>()->SetLocalPos(Float3(-10, 0, 0));
	GetComponent<Component::Quad2d>()->CreateLocalMtx();
	GetComponents<Component::Quad2d>(1)->SetLocalPos(Float3(10, 0, 0));
	GetComponents<Component::Quad2d>(1)->CreateLocalMtx();
	AddComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::eQuad2dAlha, 40, 0, Float3(1), Float3(0));
	AddComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::eQuad2dAlha, 40, 0, Float3(1), Float3(0));

	SetIsActive(false);
}

void Score::ObjectInit()
{
}

void Score::ObjectUpdate()
{
	if (GetComponent<Component::Quad2d>()->GetAlha() <= 0)
		mObjectState = ObjectState::eDead;
}

void Score::SetScoreNum(int _diceNum, int _diceCnt, int _chain)
{
	AddComponent<Component::Number>()->Init();
	AddComponent<Component::Number>()->Init();
	GetComponents<Component::Number>(0)->SetNum(_diceNum*_diceCnt);
	GetComponents<Component::Number>(1)->SetNum(_chain);

	std::cout << "　チェイン数：" << _chain;

	SetIsActive(true);
}