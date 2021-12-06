#include "skydome.h"

void Skydome::ObjectInit()
{
	// âÒì]çsóÒçÏê¨
	DX11MtxRotationX(0.05f, mRotationMtx);
}

void Skydome::ObjectUpdate()
{
	// ä|ÇØéZ
	DX11MtxMultiply(mTransform.worldMtx, mRotationMtx, mTransform.worldMtx);
}

void Skydome::ObjectDraw()
{
}

void Skydome::Uninit()
{
}

void Skydome::SetType(Type _type)
{
	switch (_type)
	{
	case Skydome::TITLE:
		GetComponent<Component::ModelComponent>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/ColorfulSkydome.fbx"));
		break;
	case Skydome::GAME:
		GetComponent<Component::ModelComponent>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/ColorfulSkydome.fbx"));
		break;
	case Skydome::CLEATE:
		GetComponent<Component::ModelComponent>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/skydome/SelectSkydome.fbx"));
		break;
	}
}