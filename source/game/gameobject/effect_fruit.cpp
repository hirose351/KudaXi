#include	"effect_fruit.h"
#include	"../component/billbord_component.h"
#include	"../component/easing_component.h"
#include	 <algorithm>

using namespace Effect;
using namespace DirectX;

Fruit::Fruit() :GameObject(("Fruit"), ObjectType::eObstracle, true)
{
	Component::Billbord* bilbord = AddComponent<Component::Billbord>();
	bilbord->LoadTexture("assets/image/effect/fruit.png");
	bilbord->SetScale(XMFLOAT2(7.0f, 7.0f));
	bilbord->SetDivUV(XMFLOAT2(6.0f, 1.0f));
	bilbord->SetOrderInLayer(78);
}

Fruit::~Fruit()
{
	mParticle.clear();
	mParticle.shrink_to_fit();
}

void Fruit::ObjectInit()
{
}

void Fruit::ObjectUpdate()
{
	XMFLOAT3 camera = Camera::GetInstance()->GetEye();
	// イテレート中に要素削除をするような場合には、範囲for文は使用できない
	for (auto itr = mParticle.begin(); itr != mParticle.end();)
	{
		(*itr).pos.x += (*itr).velocity.x;
		(*itr).velocity.y += (*itr).gravity;	// 重力に引っ張られる
		(*itr).pos.y += (*itr).velocity.y;

		if ((*itr).pos.y < (*itr).height)
		{   // 地面に当たった時の処理                                 
			(*itr).pos.y = (*itr).height;
			(*itr).velocity.y = -(*itr).attenuation * (*itr).velocity.y;
		}

		(*itr).pos.z += (*itr).velocity.z;

		// 条件一致した要素を削除する
		if ((*itr).lifetime < 0)
		{
			// 削除された要素の次を指すイテレータが返される。
			itr = mParticle.erase(itr);
		}
		// 要素削除をしない場合に、イテレータを進める
		else
		{
			(*itr).cameradistance = sqrt(pow((*itr).pos.x - camera.x, 2) + pow((*itr).pos.y - camera.y, 2) + pow((*itr).pos.z - camera.z, 2));
			(*itr).lifetime--;
			++itr;
		}
	}

	// priceをキーに降順でソート
	std::sort(mParticle.begin(), mParticle.end(), ParticleLess{});//降順ソート

	Component::Billbord* bilbord = GetComponent<Component::Billbord>();
	for (auto itr : mParticle)
	{
		//mTransform->SetPositionXYZ(itr.pos);
		bilbord->SetUV(XMFLOAT2(static_cast<float>(itr.uv), 0));
		bilbord->SetDrawPos(itr.pos);
		bilbord->Update();
	}
	for (auto itr : mParticle)
	{
		if (itr.lifetime > 0)
			return;
	}
	mObjectState = ObjectState::eDead;
}

void Fruit::SetInit(const Float3& _pos, int _uvnum)
{
	XMFLOAT3 camera = Camera::GetInstance()->GetEye();

	mCenterPos = _pos;
	AddComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::eBilbordAlha, 500, 0, Float3(1), Float3(0));

	for (int i = 0; i < 10; i++)
	{
		mParticle.emplace_back();

		mParticle.back().uv = _uvnum;
		mParticle.back().pos.x = _pos.x + (((float)rand() / (float)RAND_MAX - 0.5f) /** 0.5f*/);
		mParticle.back().pos.y = _pos.y;
		mParticle.back().pos.z = _pos.z + (((float)rand() / (float)RAND_MAX - 0.5f)/* * 0.5f*/);

		mParticle.back().lifetime = 500;
		//m_particle[i].velocity.x = ((float)rand() / (float)RAND_MAX - 0.5f) * 0.05f; //ランダムに速度を設定
		//m_particle[i].velocity.y = -((float)rand() / (float)RAND_MAX) * 10.0f;
		//m_particle[i].velocity.z = ((float)rand() / (float)RAND_MAX - 0.5f) * 0.05f;
		mParticle.back().velocity.x = (_pos.x - mParticle.back().pos.x) / 2.0f; //ランダムに速度を設定
		mParticle.back().velocity.y = -((float)rand() / (float)RAND_MAX) * 10.0f;
		mParticle.back().velocity.z = (_pos.z - mParticle.back().pos.z) / 2.0f;

		mParticle.back().height = 3.0f;
		mParticle.back().gravity = -0.5f;
		mParticle.back().attenuation = 0.5f;

		mParticle.back().cameradistance = sqrt(pow(mParticle.back().pos.x - camera.x, 2) + pow(mParticle.back().pos.y - camera.y, 2) + pow(mParticle.back().pos.z - camera.z, 2));
	}

	// priceをキーに降順でソート
	std::sort(mParticle.begin(), mParticle.end(), ParticleLess{});//降順ソート
}
