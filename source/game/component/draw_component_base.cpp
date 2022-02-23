#include		"draw_component_base.h"
#include		"../manager/scene_manager.h"

DrawComponentBase::DrawComponentBase() :ComponentBase(("DrawBase")), mOrderInLayer(0), mIsDraw(true)
{
	mSceneName = SceneManager::GetInstance()->GetCurrentSceneKey();
	// �}�l�[�W���[�Ɏ��g��o�^
	SceneManager::GetInstance()->GetScene(mSceneName)->AddDrawComponent(this);
	DX11MakeWorldMatrix(mLocalMtx, mLocalAngle, mLocalPos);
}

DrawComponentBase::~DrawComponentBase()
{
	if (SceneManager::GetInstance()->GetScene(mSceneName) == nullptr)
		return;
	SceneManager::GetInstance()->GetScene(mSceneName)->RemoveDrawComponent(this);
}

void DrawComponentBase::ImguiDraw()
{
	if (ImGui::TreeNode("DrawComponentInfo"))
	{
		ImGui::DragInt("OrderInLayer", &mOrderInLayer, 0.5f);
		ImGui::DragFloat3("LocalPosition", &mLocalPos.x, 0.5f);
		ImGui::DragFloat3("LocalAngle", &mLocalAngle.x, 0.5f);
		ImGui::TreePop();
	}
	ImguiDrawComponent();
}

void DrawComponentBase::CreateLocalMtx()
{
	// ���[�J�����W�ƃ��[�J���p�x��0�Ȃ玝����̃��[���h�s������R�s�[
	if (mLocalPos == 0 && mLocalAngle == 0)
	{
		mWorldMtx = mpOwner->GetTransform()->GetMtx();
		return;
	}
	// ���[�J���s����쐬
	DX11MakeWorldMatrix(mLocalMtx, mLocalAngle, mLocalPos);
	DX11MtxMultiply(mWorldMtx, mLocalMtx, mpOwner->GetTransform()->GetMtx());
}

void DrawComponentBase::SetDrawPos(Float3 _f3)
{
	mDrawPosList.emplace_back(_f3);
}