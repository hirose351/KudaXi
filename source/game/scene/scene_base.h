#pragma once
#include	"../../system/util/vector.h"
#include	"../manager/SceneManager.h"
#include	"XAudio2.h"

class SceneManager;
class GameObject;

class SceneBase : Uncopyable {
protected:
	SceneManager*	mManager;

	std::vector< GameObject*> mObjectList;			// �Q�[���I�u�W�F�N�g��S���܂Ƃ߂ĊǗ��ł���I�u�W�F�N�g���X�g
	std::vector< GameObject*> mPendingObjectList;	// �҂���Ԃ̃I�u�W�F�N�g

	GameObject*		mQuadfadein;
	GameObject*		mQuadfadeout;

	bool mInitingActors;	// �A�N�^�[�����������Ă��邩�ǂ���
	bool mUpdatingActors;	// �A�N�^�[���X�V���Ă��邩�ǂ���

	bool mIsPause = false;			// �|�[�Y��Ԃ�

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


	// �e�V�[���̕ύX�㏈��(�V�[�����؂芷���x�ɌĂ΂��)
	virtual void SceneAfter() = 0;
	// �e�V�[���̏�����(�������Ɉ�x�����Ă΂��)
	virtual void SceneInit() = 0;
	// �e�V�[���̍X�V
	virtual void SceneUpdate() = 0;
	// �e�V�[���̕`��
	virtual void SceneRender() = 0;

	// imgui�f�o�b�O
	virtual void ImguiDebug()/* = 0*/;
	// �I��
	virtual bool Dispose();

	void SetIsPause(bool _flg) { mIsPause = _flg; }
	bool GetIsPause() { return mIsPause; }

	// �t�F�[�h����
	virtual void UpdateFadeIn(double t);
	virtual void UpdateFadeOut(double t);

	virtual void DrawFadeIn(double t);
	virtual void DrawFadeOut(double t);
};