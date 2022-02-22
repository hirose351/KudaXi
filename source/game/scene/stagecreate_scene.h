#pragma once
#include "scene_base.h"
#include "../data/stage_data.h"

class Stage;
class StageCreateScene : public SceneBase
{
private:
	std::vector<Dix::wp<GameObject>> mpViewObjList;	// �N���G�C�g�\������I�u�W�F�N�g���X�g

	char mStageNameText[128] = {};	// �v���C���[�����͂���p�X�e�[�W��
	Dix::wp<StageData> mpStageData;
	Dix::wp<Stage> mpStage;
	int mSelectObjNum;				// �I������Ă���I�u�W�F�N�g�̐���
	bool mIsPlay = false;			// �v���C���Ă��邩
	bool mIsStagePlay = false;		// �X�e�[�W�f�[�^���v���C���Ă��邩
	std::string mBeforeStageName;	// �v���C��I���������ɓ��͂���Ă����X�e�[�W��

	void StageDataSave();
	void StageDataLoad();
	void StageDataPlay();
	void StagePlay();
	void RuleImGuiDraw();

public:
	StageCreateScene();

	void SceneAfter() override;
	void SceneInit() override;
	void SceneUpdate() override;
	void ImguiDebug() override;
};