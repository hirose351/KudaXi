#include	"gamemode_state_select.h"
#include	"../gameobject/ui_image.h"
#include	"../component/quad2d_component.h"
#include	"../manager/input_manager.h"
#include	"../manager/dice_manager.h"

using namespace GameModeState;

Select::Select()
{

	Dix::sp<myUI::ButtonGroup> b;
	b.SetPtr(new myUI::ButtonGroup);
	b->GetTransform()->SetPositionXYZ(Float3(15, 500, 0));
	b->SetInitState("assets/image/ui/number.png", 10, 1, 1, ButtonTransition::eColorTint, XMFLOAT2(0, 0), XMFLOAT2(50, 50), XMFLOAT2(70, 70), ButtonArrangement::eHorizontal, StartPoint::eLeftUp, true, 10);
	b->SetIsActive(false);
	mButton = b;
	mButton->SetInitSelectNum(0);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(b);


}

Select::~Select()
{
}

void Select::Exec()
{
	// ���͂ɑ΂��鏈���i���͂���Ă�����X�e�[�W�A�v���C���[�A�T�C�R���X�V�j

	// �I������Ă���X�e�[�W�ԍ����ύX���ꂽ��X�e�[�W�����X�V����
	if (mStageNum != mButton->GetSelectNum() + 1)
	{
		mStageNum = mButton->GetSelectNum() + 1;
		if (mStageNum / 10 > 0)
		{
			mStr = "puzzle/" + std::to_string(mStageNum);
		}
		else
		{
			mStr = "puzzle/0" + std::to_string(mStageNum);
		}

		StageDataManager::GetInstance().SetCurrentStage(mStr);
		Dix::wp<StageData> stageData;
		stageData = StageDataManager::GetInstance().GetCurrentStage();

		DiceManager::GetInstance()->DiceMapCreate(false);

		Camera::GetInstance()->SetLookat(XMFLOAT3(stageData->mMapSizeWidth*stageData->mMapChipSize / 2.0f, 0, -57.5f - stageData->mMapSizeHeight*stageData->mMapChipSize / 2.0f));
		Camera::GetInstance()->SetEye(XMFLOAT3(stageData->mMapSizeWidth*stageData->mMapChipSize / 2.0f, 250, -197.5f - stageData->mMapSizeHeight*stageData->mMapChipSize / 2.0f));
		Camera::GetInstance()->CreateCameraMatrix();
	}



	// �����ꂽ�u�Ԃ̏���
	InputDirection i = InputManager::GetInstance().GetDirectionTrigger(InputMode::eUi, static_cast<int>(UiAction::eNavigate));

	// �n�C���C�g&�傫���ύX����Ă�ԍ��̕ύX����



	// ����������ꂽ�Ƃ��̏����i�X�e�[�W�͂��̂܂܂ŃJ�����𓮂��������j
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eClick)))
	{

	}

}

void Select::BeforeChange()
{
	SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(true);
	// �J�����̈ʒu�ς��悤
	if (mStageNum / 10 > 0)
	{
		mStr = "puzzle/" + std::to_string(mStageNum);
	}
	else
	{
		mStr = "puzzle/0" + std::to_string(mStageNum);
	}

	DiceManager::GetInstance()->DiceMapCreate(false);

	StageDataManager::GetInstance().SetCurrentStage(mStr);
	Dix::wp<StageData> stageData;
	stageData = StageDataManager::GetInstance().GetCurrentStage();

	Camera::GetInstance()->SetLookat(XMFLOAT3(stageData->mMapSizeWidth*stageData->mMapChipSize / 2.0f, 0, -57.5f - stageData->mMapSizeHeight*stageData->mMapChipSize / 2.0f));
	Camera::GetInstance()->SetEye(XMFLOAT3(stageData->mMapSizeWidth*stageData->mMapChipSize / 2.0f, 250, -197.5f - stageData->mMapSizeHeight*stageData->mMapChipSize / 2.0f));
	Camera::GetInstance()->CreateCameraMatrix();

	mButton->SetIsActive(true);
}

void Select::AfterChange()
{
	SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(false);
	mButton->SetIsActive(false);
}
