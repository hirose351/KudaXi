#include	"SceneManager.h"

// �ЂƂ̑J�ڕ��@�ōX�V
void SceneManager::UpdateSingle()
{
	// �o�ߎ��Ԃ��擾
	double elapsed = mStopwatch.msF();

	// �t�F�[�h�A�E�g�J�ځ@and �o�ߎ��Ԃ��J�ڎ��Ԃ��I�[�o�[�����Ƃ�
	if (mTransitionState == TransitionState::FadeOut
		&& elapsed >= mTransitionTimeMillisec)
	{
		// ���̃V�[���̍X�V���������s
		mScenefactories[mNextscenekey]->Update();

		// �J�����g�V�[�������̃V�[���ɂ���
		mCurrentscenekey = mNextscenekey;

		// �J�ڃX�e�[�g���t�F�[�h�C���ɂ���
		mTransitionState = TransitionState::FadeIn;

		// �X�g�b�v�E�H�b�`���ăX�^�[�g
		mStopwatch.restart();

		// �o�ߎ��Ԃ��O�ɂ��ǂ�
		elapsed = 0.0;
	}

	// �t�F�[�h�C���J�ځ@and �o�ߎ��Ԃ��J�ڎ��Ԃ��I�[�o�[�����Ƃ�
	if (mTransitionState == TransitionState::FadeIn &&
		elapsed >= mTransitionTimeMillisec)
	{
		// �X�g�b�v�E�H�b�`�����Z�b�g
		mStopwatch.reset();

		// �J�ڎ��Ԃ��`�b�s�h�u�d�ɂ���
		mTransitionState = TransitionState::Active;
	}

	switch (mTransitionState)
	{
	case TransitionState::FadeIn:
		assert(mTransitionTimeMillisec);
		mScenefactories[mCurrentscenekey]->UpdateFadeIn(elapsed / mTransitionTimeMillisec);
		return;
	case TransitionState::Active:
		mScenefactories[mCurrentscenekey]->Update();
		return;
	case TransitionState::FadeOut:
		assert(mTransitionTimeMillisec);
		mScenefactories[mCurrentscenekey]->UpdateFadeOut(elapsed / mTransitionTimeMillisec);
		return;
	default:
		return;
	}
}

void SceneManager::UpdateCross()
{
	// �o�ߎ��Ԃ��擾
	const double elapsed = mStopwatch.msF();

	// �J�ڕ��@���N���X�t�F�[�h�̏ꍇ
	if (mTransitionState == TransitionState::FadeInOut)
	{
		if (elapsed >= mTransitionTimeMillisec)
		{
			mCurrentscenekey = mNextscenekey;

			mStopwatch.reset();

			mTransitionState = TransitionState::Active;
		}
	}

	if (mTransitionState == TransitionState::Active)
	{
		mScenefactories[mCurrentscenekey]->Update();

		return;
	}
	else
	{
		const double t = elapsed / mTransitionTimeMillisec;

		mScenefactories[mCurrentscenekey]->UpdateFadeOut(t);

		mScenefactories[mNextscenekey]->UpdateFadeIn(t);

		return;
	}
}


// �J�����g�V�[�����Z�b�g����
void SceneManager::setCurrentScene(std::string key) {

	mBeforescenekey = mCurrentscenekey;
	mCurrentscenekey = key;
}

void SceneManager::Update() {

	if (mCrossFade)
	{
		return UpdateCross();
	}
	else
	{
		return UpdateSingle();
	}
}

void SceneManager::Render() {

	DrawScene();

	// �J�����g�V�[���`��
//		m_scenefactories[m_currentscenekey]->Draw();
}



void SceneManager::DrawScene()
{
	// �J�����g�V�[���L�[����Ȃ牽�����Ȃ�
	if (mCurrentscenekey.empty())
	{
		return;
	}

	// �A�N�e�B�u���J�ڎ��Ԃ��O�Ȃ猻�݃V�[����`��
	if (mTransitionState == TransitionState::Active ||
		!mTransitionTimeMillisec)
	{
		mScenefactories[mCurrentscenekey]->Render();
	}

	// �o�ߎ��Ԃ��擾
	const double elapsed = mStopwatch.msF();
	// �J�ڃt�F�[�h�C���Ȃ�
	if (mTransitionState == TransitionState::FadeIn)
	{
		// �t�F�[�h�C���`��

//		TurnOffZbuffer();
		mScenefactories[mCurrentscenekey]->DrawFadeIn(elapsed / mTransitionTimeMillisec);
		//		TurnOnZbuffer();
	}
	// �J�ڃt�F�[�h�A�E�g�Ȃ�
	else if (mTransitionState == TransitionState::FadeOut)
	{
		// �t�F�[�h�A�E�g�`��
//		TurnOffZbuffer();
		mScenefactories[mCurrentscenekey]->DrawFadeOut(elapsed / mTransitionTimeMillisec);
		//		TurnOnZbuffer();
	}
	// �J�ڃN���X�t�F�[�h�Ȃ�
	else if (mTransitionState == TransitionState::FadeInOut)
	{
		// �t�F�[�h�A�E�g�`��
//		TurnOffZbuffer();
		mScenefactories[mCurrentscenekey]->DrawFadeOut(elapsed / mTransitionTimeMillisec);
		// ���̃V�[�����ݒ肳��Ă����
		if (!mNextscenekey.empty())
		{
			// �t�F�[�h�C���`��
			mScenefactories[mNextscenekey]->DrawFadeIn(elapsed / mTransitionTimeMillisec);
		}
		//		TurnOnZbuffer();
	}
}

void SceneManager::SetTransitionState(SceneManager::TransitionState ts) {
	mTransitionState = ts;
}


std::string SceneManager::GetCurrentSceneKey() {
	return mCurrentscenekey;
}

std::string SceneManager::GetBeforeSceneKey()
{
	return mBeforescenekey;
}

std::string SceneManager::GetAddSceneKey()
{
	return mAddkey;
}

DirectX::XMFLOAT4 SceneManager::GetFadeColor() {
	return mFadeColor;
}

void SceneManager::SetFadeColor(DirectX::XMFLOAT4 color) {
	mFadeColor = color;
}
