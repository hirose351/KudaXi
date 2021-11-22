#pragma once
#include	"../../system/util/uncopyable.h"
#include	<memory>
#include	<wrl/client.h>
#include	<unordered_map>
#include	"../data/stage_data.h"

class StageDataManager :public Uncopyable
{
private:
	StageDataManager() {
	}
	std::unordered_map<
		std::string,					// �L�[(�X�e�[�W��)
		std::unique_ptr<StageData>		// �f�[�^(StageData�N���X)
	> mStageHashmap;

	std::string mCurrentStageKey = "puzzle/init";		// ���݂̃X�e�[�W�������L�[

public:
	static StageDataManager& GetInstance() {
		static StageDataManager Instance;
		return Instance;
	}

	virtual ~StageDataManager() {
		//	Finalize();
	}

	// ��n��
	void Finalize();

	// �X�e�[�W���Z�b�g
	bool SetCurrentStage(std::string _key);

	// ���݂̃X�e�[�W�f�[�^�̃|�C���^�擾
	const StageData* GetCurrentStage();

	// �X�e�[�W�f�[�^�ǂݍ���
	bool LoadStage(std::string _key);

	// �X�e�[�W�f�[�^�ۑ�
	void SaveStage(const StageData& _stagedata);

	// �X�e�[�W�f�[�^�̃|�C���^���擾����
	const StageData* GetStageData(std::string _key);

	// �X�e�[�W�f�[�^���폜����
	void RemoveStageData(std::string _key);
};