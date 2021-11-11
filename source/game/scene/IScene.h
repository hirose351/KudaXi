#pragma once
#include	"../manager/SceneManager.h"
#include	"XAudio2.h"

class SceneManager;

class IScene : Uncopyable {
private:
	SceneManager*	mManager;
public:
	IScene();
	void SetSceneManager(SceneManager* _sm) {
		mManager = _sm;
	}

	virtual ~IScene() {}

	// ������
	virtual void Init() = 0;
	// �X�V
	virtual void Update() = 0;
	// �`��
	virtual void Render() = 0;
	// �f�o�b�O
	virtual void imguidebug() = 0;
	// �I��
	virtual void Dispose() = 0;

	virtual void updateFadeIn(double t);
	virtual void updateFadeOut(double t);

	virtual void drawFadeIn(double t);
	virtual void drawFadeOut(double t);
};