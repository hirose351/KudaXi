#include	"gamemode_state_puzzle.h"
#include	"../gameobject/ui_image.h"
#include	"../manager/stagedata_manager.h"
#include	"../manager/dice_manager.h"
#include	"../manager/input_manager.h"

using namespace GameModeState;

Puzzle::Puzzle()
{
	// StageStringUI
	Dix::sp<myUI::Image> stageString;
	stageString.SetPtr(new myUI::Image);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stageString);
	mUiStage = stageString->AddComponent<Component::Quad2d>();
	mUiStage->SetInfo("assets/image/ui/stage.png", XMFLOAT4(1, 1, 1, 1));
	mUiStage->SetOrderInLayer(1);
	mModeObjList.emplace_back(stageString);

	// StageNumUI
	Dix::sp<myUI::Image> stageNum;
	stageNum.SetPtr(new myUI::Image);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stageNum);
	mUiStageNum = stageNum->AddComponent<Component::Quad2d>();
	mUiStageNum->SetInfo("assets/image/ui/number.png", XMFLOAT4(1, 1, 1, 1), 10);
	mUiStageNum->SetOrderInLayer(1);
	mModeObjList.emplace_back(stageNum);

	// StepStringUI
	Dix::sp<myUI::Image> stepString;
	stepString.SetPtr(new myUI::Image);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(stepString);
	mUiStep = stepString->AddComponent<Component::Quad2d>();
	mUiStep->SetInfo("assets/image/ui/step.png", XMFLOAT4(1, 1, 1, 1));
	mUiStep->SetOrderInLayer(1);
	mModeObjList.emplace_back(stepString);

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
	clearOver.SetPtr(new myUI::Image);
	clearOver->GetTransform()->SetScale(Float3(500.0f, 250.0f, 0));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(clearOver);
	mUiClearOver = clearOver->AddComponent<Component::Quad2d>();
	mUiClearOver->SetInfo("assets/image/ui/clearover.png", XMFLOAT4(1, 1, 1, 1), 1, 2);
	mUiClearOver->SetOrderInLayer(1);
	mModeObjList.emplace_back(clearOver);

	// ���̃X�e�[�W�ցA�X�e�[�W�Z���N�g�ցA������x

	mUiStage->SetIsDraw(false);
	mUiStageNum->SetIsDraw(false);
	mUiStep->SetIsDraw(false);
	mUiStepNum->SetIsDraw(false);
	mUiClearOver->SetIsDraw(false);
}

Puzzle::~Puzzle()
{
}

void Puzzle::Exec()
{
	// �v���C���[or�T�C�R���̍s���ɂ���ăX�e�b�v�����炷

	// �߂�������ꂽ�Ƃ��̏���
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eCancel)))
	{
		mHolder->ChangeMode(eSelect);
	}
}

void Puzzle::BeforeChange()
{
	//for (Dix::wp<GameObject> obj : mModeObjList)
	//{
	//	obj->SetIsActive(true);
	//}

	mUiStage->SetIsDraw(true);
	mUiStageNum->SetIsDraw(true);
	mUiStep->SetIsDraw(true);
	mUiStepNum->SetIsDraw(true);
	// �Z���N�g�őI�����ꂽ�X�e�b�v�����擾
	StageData data = StageDataManager::GetInstance().GetCurrentStage().At();

	mStep = data.mStep;
	// �X�e�[�W�ԍ��X�V
	mUiStageNum->SetUvPos(INT2(mHolder->GetSelectStage(), 0));
	// �X�e�b�v�ԍ��X�V
	mUiStepNum->SetUvPos((data.mStep, 0));
	// �N���A�I�[�o�[��\��
	mUiClearOver->SetIsDraw(false);
	// �T�C�R���̏�Ԃ�ʏ�ɖ߂�
	DiceManager::GetInstance()->SetPuzzle();
}

void Puzzle::AfterChange()
{
	mUiStage->SetIsDraw(false);
	mUiStageNum->SetIsDraw(false);
	mUiStep->SetIsDraw(false);
	mUiStepNum->SetIsDraw(false);
	mUiClearOver->SetIsDraw(false);
}
