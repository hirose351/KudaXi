#pragma once
#include	<string>
#include	<unordered_map>
#include	<functional>
#include	<iostream>
#include	<memory>
#include	<directxmath.h>
#include	<chrono>
#include	<thread>
#include	"../../Application.h"
#include	"../../system/util/uncopyable.h"
#include	"../../system/util/StopWatch.h"
#include	"../../system/util/quad2d.h"
#include	"../scene/scene_base.h"

class SceneBase;

class SceneManager :public Uncopyable {
private:
	// �J�ڕ��@��`�񋓃N���X
	enum class TransitionState
	{
		None,
		FadeIn,
		Active,
		FadeOut,
		FadeInOut,
	};

	// �V�[���ۊ�
	std::unordered_map<std::string, std::unique_ptr<SceneBase>>	mScenefactories;
	std::string mBeforescenekey;			// �O�V�[���̃L�[
	std::string mCurrentscenekey;			// �J�����g�V�[���̃L�[
	std::string mNextscenekey;				// ���V�[��

	// ��ʑJ�ږ@
	TransitionState mTransitionState = TransitionState::None;

	int32_t	mTransitionTimeMillisec = 1000;	// �J�ڎ��ԁi�f�t�H���g�F�P�O�O�O�~���b�j

	Stopwatch mStopwatch;						// �X�g�b�v�E�I�b�`

	DirectX::XMFLOAT4 mFadeColor = DirectX::XMFLOAT4(0, 0, 0, 1);		// �t�F�[�h�J���[

	bool mCrossFade = false;

	bool mGameEndFlg = false;	// �Q�[�����I�����邩

	void UpdateSingle();					// �ЂƂ̑J�ڕ��@�ōX�V
	void UpdateCross();						// �N���X�t�F�[�h�ōX�V
	void DrawScene();						// �J�����g�V�[���`��
	void SetTransitionState(SceneManager::TransitionState ts);	// �J�ڕ��@�Z�b�g

public:

	static SceneManager* GetInstance() {
		static SceneManager Instance;
		return &Instance;
	}

	bool GetGameEndFlg() {
		return mGameEndFlg;
	}
	void SetGameEndFlg(bool _flg) {
		mGameEndFlg = _flg;
	}

	template <class sceneclass>
	void add(std::string key);
	// �V�[����ύX����1
	template <class sceneclass>
	void changeScene(std::string key);

	// �V�[����ύX����2
	template <class sceneclass>
	bool changeScene(std::string key, int32_t transitionTimeMillisec, bool crossFade);

	// �J�����g�V�[�����Z�b�g����
	void setCurrentScene(std::string key);
	void Update();
	void Render();

	std::string GetCurrentSceneKey();
	std::string GetBeforeSceneKey();
	DirectX::XMFLOAT4 GetFadeColor();
	void SetFadeColor(DirectX::XMFLOAT4 color);
};

template <class sceneclass>
void SceneManager::add(std::string key) {
	mScenefactories[key] = std::move(std::make_unique<sceneclass>());
	mScenefactories[key].get()->Init();
	mScenefactories[key].get()->SetSceneManager(this);

	SetTransitionState(SceneManager::TransitionState::Active);
}

// �V�[����ύX����
template <class sceneclass>
void SceneManager::changeScene(std::string key) {

	// �w�肳�ꂽ�L�[�����݂��Ă��邩
	auto it = mScenefactories.find(key);
	if (it != mScenefactories.end())
	{
		// ���݂��Ă����ꍇ
		mScenefactories[key].get()->Init(this);
		mScenefactories[key].get()->SetSceneManager(this);
	}
	else
	{
		// ���݂��Ă��Ȃ������ꍇ����
		mScenefactories[key] = std::move(std::make_unique<sceneclass>());
		mScenefactories[key].get()->Init(this);
		mScenefactories[key].get()->SetSceneManager(this);
	}
	// �J�����g�V�[���Z�b�g	
	setCurrentScene(key);
}

template <class sceneclass>
bool SceneManager::changeScene(std::string key, int32_t transitionTimeMillisec, bool crossFade) {
	// �O�̃V�[�����J�����g�V�[���ɂ���
	mBeforescenekey = mCurrentscenekey;

	// ���݂̃V�[���ƕύX��̃V�[���������ł���΃N���X�t�F�[�h�s��Ȃ�
	if (key == mCurrentscenekey)
	{
		crossFade = false;
	}
	// �w�肳�ꂽ�L�[�����݂��Ă��邩
	auto it = mScenefactories.find(key);
	if (it != mScenefactories.end())
	{
		// ���݂��Ă����ꍇ
		// �V�[��������
		mScenefactories[key].get()->Init();
		// �V�[���}�l�[�W���̃|�C���^���Z�b�g
		mScenefactories[key].get()->SetSceneManager(this);
	}
	else
	{
		// ���݂��Ă��Ȃ������ꍇ����
		mScenefactories[key] = std::move(std::make_unique<sceneclass>());
		// �V�[��������
		mScenefactories[key].get()->Init();
		// �V�[���}�l�[�W���̃|�C���^���Z�b�g
		mScenefactories[key].get()->SetSceneManager(this);
	}
	mNextscenekey = key;
	mCrossFade = crossFade;
	// �N���X�t�F�[�h���s��
	if (crossFade)
	{
		// �J�ڎ��Ԃ������o�ϐ��ɃZ�b�g
		mTransitionTimeMillisec = transitionTimeMillisec;
		mTransitionState = TransitionState::FadeInOut;
		mStopwatch.restart();
	}
	else
	{
		//�@�t�F�[�h�A�E�g��Ƀt�F�[�h�C���Ȃ̂őJ�ڎ��Ԃ�2�Ŋ���
		mTransitionTimeMillisec = (transitionTimeMillisec / 2);
		mTransitionState = TransitionState::FadeOut;
		mStopwatch.restart();
	}

	return true;
}