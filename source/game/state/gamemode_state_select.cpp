#include	"gamemode_state_select.h"
#include	"../gameobject/ui_image.h"
#include	"../manager/input_manager.h"
#include	"../manager/dice_manager.h"
#include	"../component/quad2d_component.h"
#include	"../component/player_controller.h"
#include	"../component/map_pos_component.h"

using namespace GameModeState;

void Select::SetStage()
{
	std::string str;
	if (mStageNum / 10 > 0)
		str = "puzzle/" + std::to_string(mStageNum);
	else
		str = "puzzle/0" + std::to_string(mStageNum);

	StageDataManager::GetInstance().SetCurrentStage(str);
	Dix::wp<StageData> stageData;
	stageData = StageDataManager::GetInstance().GetCurrentStage();

	DiceManager::GetInstance()->DiceMapCreate(false);
	mHolder->GetStage()->Init();
	mHolder->GetPlayer()->GetComponent<Component::MapPos>()->SetMapPosMove(stageData->mPlayerPos);

	if (DiceManager::GetInstance()->GetDice(INT3(stageData->mPlayerPos.x, 0, stageData->mPlayerPos.z)) != NULL)
		mHolder->GetPlayer()->GetTransform()->SetPositionY(DICE_SCALE + mHolder->GetPlayer()->GetTransform()->scale.y);
	else
		mHolder->GetPlayer()->GetTransform()->SetPositionY(mHolder->GetPlayer()->GetTransform()->scale.y / 2.0f);

	// �J�������Z�b�g����Ă��Ȃ���
	if (!mHolder->GetIsSetCamera())
	{
		Camera::GetInstance()->SetLookat(XMFLOAT3(stageData->mMapSizeWidth*stageData->mMapChipSize / 2.0f, 0, -stageData->mMapSizeHeight*stageData->mMapChipSize / 2.0f));
		Camera::GetInstance()->SetEye(XMFLOAT3(stageData->mMapSizeWidth*stageData->mMapChipSize / 2.0f, 250, -197.5f - stageData->mMapSizeHeight*stageData->mMapChipSize / 2.0f));
		Camera::GetInstance()->CreateCameraMatrix();
	}
	else
		mHolder->SetIsSetCamera(false);
}

Select::Select()
{
	// �{�^��
	Dix::sp<myUI::ButtonGroup> b;
	b.SetPtr(new myUI::ButtonGroup);
	b->GetTransform()->SetPositionXYZ(Float3(150, 500, 0));
	b->SetInitState("assets/image/ui/number.png", 10, 1, 10, ButtonTransition::eColorTint, XMFLOAT2(50, 0), XMFLOAT2(50, 50), XMFLOAT2(70, 70), ButtonArrangement::eHorizontal, StartPoint::eLeftUp, true, 2);
	mButton = b; mButton->SetInitSelectNum(0);
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(b);
	mModeObjList.emplace_back(mButton);

	// ����
	Dix::sp<myUI::Image> selectString;
	selectString.SetPtr(new myUI::Image);
	selectString->GetTransform()->SetScale(Float3(400));
	SceneManager::GetInstance()->GetCurrentScene()->AddGameObject(selectString);
	Component::Quad2d* uiStageQuad = selectString->AddComponent<Component::Quad2d>();
	uiStageQuad->SetInfo("assets/image/ui/stageselect.png", XMFLOAT4(1, 1, 1, 1));
	uiStageQuad->SetOrderInLayer(1);
	selectString->GetTransform()->SetPositionXYZ(Float3(1280 / 2.0f, 100.0f, 0));
	mModeObjList.emplace_back(selectString);

	for (Dix::wp<GameObject> obj : mModeObjList)
		obj->SetIsActive(false);
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
		SetStage();
	}

	// ����������ꂽ�Ƃ��̏����i�X�e�[�W�͂��̂܂܂ŃJ�����𓮂��������j
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eClick)))
	{
		mHolder->SetSelectStage(mButton->GetSelectNum() + 1);
		mHolder->ChangeMode(ePuzzle);
	}
	// �߂�������ꂽ�Ƃ��̏���
	else if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eCancel)))
	{
		AfterChange();
		SceneManager::GetInstance()->SetNextScene("Mode");
	}
}

void Select::BeforeChange()
{
	SetStage();

	for (Dix::wp<GameObject> obj : mModeObjList)
		obj->SetIsActive(true);

	mButton->SetInitSelectNum(mStageNum - 1);

	mHolder->GetPlayer()->GetComponent<Component::PlayerController>()->RemoveDiceUi();
	SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(true);
}

void Select::AfterChange()
{
	SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(false);
	mHolder->SetIsSetCamera(false);

	for (Dix::wp<GameObject> obj : mModeObjList)
		obj->SetIsActive(false);
}
