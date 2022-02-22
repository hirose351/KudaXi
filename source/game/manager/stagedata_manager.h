#pragma once
#include	"../../system/util/uncopyable.h"
#include	"../../system/util/dixsmartptr.h"
#include	<unordered_map>
#include	"../data/stage_data.h"

class StageDataManager :public Uncopyable
{
private:
	std::unordered_map<
		std::string,					// �L�[(�X�e�[�W��)
		Dix::sp<StageData>				// �f�[�^(StageData�N���X)
	> mpStageHashmap;

	std::string mCurrentStageKey = "puzzle/init";		// ���݂̃X�e�[�W�������L�[

public:
	static StageDataManager& GetInstance() {
		static StageDataManager Instance;
		return Instance;
	}

	// �X�e�[�W���Z�b�g
	bool SetCurrentStage(std::string _key);

	// ���݂̃X�e�[�W�f�[�^�̃|�C���^�擾
	const Dix::wp<StageData> GetCurrentStage();

	// �X�e�[�W�f�[�^�ǂݍ���
	bool LoadStage(std::string _key);

	// �X�e�[�W�f�[�^�ۑ�
	void SaveStage(const StageData& _stagedata);

	// �X�e�[�W�f�[�^�̃|�C���^���擾����
	const Dix::wp<StageData> GetStageData(std::string _key);

	// �X�e�[�W�f�[�^���폜����
	void RemoveStageData(std::string _key);
};