#include	"SceneManager.h"

// ひとつの遷移方法で更新
void SceneManager::UpdateSingle()
{
	// 経過時間を取得
	double elapsed = mStopwatch.msF();

	// フェードアウト遷移　and 経過時間が遷移時間をオーバーしたとき
	if (mTransitionState == TransitionState::eFadeOut
		&& elapsed >= mTransitionTimeMillisec)
	{
		// 次のシーンの更新処理を実行
		mScenefactories[mNextscenekey]->Update();

		// カレントシーンを次のシーンにする
		mCurrentscenekey = mNextscenekey;

		// 遷移ステートをフェードインにする
		mTransitionState = TransitionState::eFadeIn;

		// ストップウォッチを再スタート
		mStopwatch.restart();

		// 経過時間を０にもどす
		elapsed = 0.0;
	}

	// フェードイン遷移　and 経過時間が遷移時間をオーバーしたとき
	if (mTransitionState == TransitionState::eFadeIn &&
		elapsed >= mTransitionTimeMillisec)
	{
		// ストップウォッチをリセット
		mStopwatch.reset();

		// 遷移時間をＡＣＴＩＶＥにする
		mTransitionState = TransitionState::eActive;
	}

	switch (mTransitionState)
	{
	case TransitionState::eFadeIn:
		assert(mTransitionTimeMillisec);
		mScenefactories[mCurrentscenekey]->UpdateFadeIn(elapsed / mTransitionTimeMillisec);
		return;
	case TransitionState::eActive:
		mScenefactories[mCurrentscenekey]->Update();
		return;
	case TransitionState::eFadeOut:
		assert(mTransitionTimeMillisec);
		mScenefactories[mCurrentscenekey]->UpdateFadeOut(elapsed / mTransitionTimeMillisec);
		return;
	default:
		return;
	}
}

void SceneManager::UpdateCross()
{
	// 経過時間を取得
	const double elapsed = mStopwatch.msF();

	// 遷移方法がクロスフェードの場合
	if (mTransitionState == TransitionState::eFadeInOut)
	{
		if (elapsed >= mTransitionTimeMillisec)
		{
			mCurrentscenekey = mNextscenekey;

			mStopwatch.reset();

			mTransitionState = TransitionState::eActive;
		}
	}

	if (mTransitionState == TransitionState::eActive)
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


// カレントシーンをセットする
void SceneManager::setCurrentScene(std::string key) {

	mBeforescenekey = mCurrentscenekey;
	mCurrentscenekey = key;
	mAddkey = mCurrentscenekey;
	//mScenefactories[key].get()->SceneInit();
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

	// カレントシーン描画
//		m_scenefactories[m_currentscenekey]->Draw();
}



void SceneManager::DrawScene()
{
	// カレントシーンキーが空なら何もしない
	if (mCurrentscenekey.empty())
	{
		return;
	}

	// アクティブかつ遷移時間が０なら現在シーンを描画
	if (mTransitionState == TransitionState::eActive ||
		!mTransitionTimeMillisec)
	{
		mScenefactories[mCurrentscenekey]->Render();
	}

	// 経過時間を取得
	const double elapsed = mStopwatch.msF();
	// 遷移フェードインなら
	if (mTransitionState == TransitionState::eFadeIn)
	{
		// フェードイン描画

//		TurnOffZbuffer();
		mScenefactories[mCurrentscenekey]->DrawFadeIn(elapsed / mTransitionTimeMillisec);
		//		TurnOnZbuffer();
	}
	// 遷移フェードアウトなら
	else if (mTransitionState == TransitionState::eFadeOut)
	{
		// フェードアウト描画
//		TurnOffZbuffer();
		mScenefactories[mCurrentscenekey]->DrawFadeOut(elapsed / mTransitionTimeMillisec);
		//		TurnOnZbuffer();
	}
	// 遷移クロスフェードなら
	else if (mTransitionState == TransitionState::eFadeInOut)
	{
		// フェードアウト描画
//		TurnOffZbuffer();
		mScenefactories[mCurrentscenekey]->DrawFadeOut(elapsed / mTransitionTimeMillisec);
		// 次のシーンが設定されていれば
		if (!mNextscenekey.empty())
		{
			// フェードイン描画
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
