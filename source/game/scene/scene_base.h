#pragma once
#include	"../manager/SceneManager.h"
#include	"XAudio2.h"

class SceneManager;
class GameObject;

class SceneBase : Uncopyable {
private:
	SceneManager*	mManager;

	std::vector<GameObject*> objectList;	// �Q�[���I�u�W�F�N�g��S���܂Ƃ߂ĊǗ��ł���I�u�W�F�N�g���X�g

	Quad2D		gQuadfadein;
	Quad2D		gQuadfadeout;
public:
	SceneBase();
	void SetSceneManager(SceneManager* _sm) {
		mManager = _sm;
	}

	virtual ~SceneBase() {}

	// ������
	virtual void AddGameObject() = 0;
	void AddGameObject(GameObject* _object);

	virtual bool Init() = 0;
	// �X�V
	virtual void Update();
	// �`��
	virtual void Render();
	// �f�o�b�O
	virtual void ImguiDebug() = 0;
	// �I��
	virtual bool Dispose();

	virtual void updateFadeIn(double t);
	virtual void updateFadeOut(double t);

	virtual void drawFadeIn(double t);
	virtual void drawFadeOut(double t);
};