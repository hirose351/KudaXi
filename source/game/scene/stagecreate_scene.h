#pragma once
#include "scene_base.h"

class StageCreateScene : public SceneBase
{
private:
	//Stage mStage;
	//Player mPlayer;
	//Skydome mSkydome;

	//char mStageNameText[128] = {};
	//bool mIsEnablePlayer = true;
	//int mGameObjectNum = 0;
	//int mDicemoveNum = 0;
	//int mDiceNum = 3;					// �_�C�X�̓Y����

	//float mLookat[3] = { 0, 0, 0 };			// �����_

public:
	StageCreateScene();

	void SceneAfter() override;
	void SceneInit() override;
	void SceneUpdate() override;
};