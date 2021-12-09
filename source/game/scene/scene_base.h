#pragma once
#include	"../manager/SceneManager.h"
#include	"../../system/util/vector.h"
#include	"XAudio2.h"

class SceneManager;
class GameObject;

class SceneBase : Uncopyable {
private:
	SceneManager*	mManager;

	std::vector<GameObject*> mObjectList;	// �Q�[���I�u�W�F�N�g��S���܂Ƃ߂ĊǗ��ł���I�u�W�F�N�g���X�g
	// �҂���Ԃ̃A�N�^�[�Q
	std::vector<GameObject*> mPendingActors;

	Quad2D		mQuadfadein;
	Quad2D		mQuadfadeout;

	// �A�N�^�[���X�V���Ă��邩�ǂ���
	bool mUpdatingActors;
protected:
	Float3 mCameraLookat;			// �J�����̒����_

public:
	SceneBase();
	void SetSceneManager(SceneManager* _sm) {
		mManager = _sm;
	}

	virtual ~SceneBase();

	// ���X�g�ɒǉ�
	void AddGameObject(GameObject* _object);
	// ���X�g����폜
	void RemoveGameObject(GameObject* _object);

	// �I�u�W�F�N�g������
	bool Init();
	// �I�u�W�F�N�g�X�V
	void Update();
	// �I�u�W�F�N�g�`��
	void Render();

	// �e�V�[���̏�����
	virtual void SceneInit() = 0;
	// �e�V�[���̍X�V
	virtual void SceneUpdate() = 0;
	// �e�V�[���̕`��
	virtual void SceneRender() = 0;

	// imgui�f�o�b�O
	virtual void ImguiDebug()/* = 0*/;
	// �I��
	virtual bool Dispose();

	// �t�F�[�h����

	virtual void UpdateFadeIn(double t);
	virtual void UpdateFadeOut(double t);

	virtual void DrawFadeIn(double t);
	virtual void DrawFadeOut(double t);
};