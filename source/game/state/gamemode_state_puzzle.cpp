#include	"gamemode_state_puzzle.h"
#include	"../gameobject/ui_image.h"
#include	"../manager/stagedata_manager.h"

using namespace GameModeState;

Puzzle::Puzzle()
{
	// StageNumUI
	Dix::sp<myUI::Image> stageNum;
	stageNum.SetPtr(new myUI::Image);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stageNum);
	mUiStageNum = stageNum->AddComponent<Component::Quad2d>();
	mUiStageNum->SetInfo("assets/image/ui/number.png", XMFLOAT4(1, 1, 1, 1), 10);
	mUiStageNum->SetOrderInLayer(1);
	mModeObjList.emplace_back(stageNum);

	// StepNumUI
	Dix::sp<myUI::Image> stepNum;
	stepNum.SetPtr(new myUI::Image);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stepNum);
	mUiStepNum = stepNum->AddComponent<Component::Quad2d>();
	mUiStepNum->SetInfo("assets/image/ui/number.png", XMFLOAT4(1, 1, 1, 1), 10);
	mUiStepNum->SetOrderInLayer(1);
	mModeObjList.emplace_back(stepNum);

	// ClearOverUI
	Dix::sp<myUI::Image> clearOver;
	clearOver->GetTransform()->SetScale(Float3(500.0f, 250.0f, 0));
	clearOver.SetPtr(new myUI::Image);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(clearOver);
	mUiClearOver = clearOver->AddComponent<Component::Quad2d>();
	mUiClearOver->SetInfo("assets/image/ui/clearover.png", XMFLOAT4(1, 1, 1, 1), 1, 2);
	mUiClearOver->SetOrderInLayer(1);
	mModeObjList.emplace_back(clearOver);

	// ���̃X�e�[�W�ցA�X�e�[�W�Z���N�g�ցA������x


}

Puzzle::~Puzzle()
{
}

void Puzzle::Exec()
{
	// �v���C���[or�T�C�R���̍s���ɂ���ăX�e�b�v�����炷

}

void Puzzle::BeforeChange()
{
	for (Dix::wp<GameObject> obj : mModeObjList)
	{
		obj->SetIsActive(true);
	}

	// �Z���N�g�őI�����ꂽ�X�e�b�v�����擾
	StageData data = StageDataManager::GetInstance().GetCurrentStage().At();

	// �X�e�[�W�ԍ��X�V
	//mStageNum->SetUvPos();
	// �X�e�b�v�ԍ��X�V
	mUiStepNum->SetUvPos((data.mStep, 0));
	// �N���A�I�[�o�[��\��
	mUiClearOver->SetIsDraw(false);
}

void Puzzle::AfterChange()
{

}
