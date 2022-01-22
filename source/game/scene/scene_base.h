#pragma once
#include	"../../system/util/vector.h"
#include	"../manager/scene_manager.h"
#include	"../manager/draw_manager.h"
#include	"../../system/util/dixsmartptr.h"
#include	"XAudio2.h"
#include	"../../Memory.h"

class SceneManager;
class GameObject;

class SceneBase : Uncopyable {
protected:
	SceneManager*	mpSceneManager;
	DrawManager mDrawManager;

	std::list<Dix::sp<GameObject>> mpObjectList;			// �Q�[���I�u�W�F�N�g��S���܂Ƃ߂ĊǗ��ł���I�u�W�F�N�g���X�g
	std::list<Dix::sp<GameObject>> mpPendingObjectList;	// �҂���Ԃ̃I�u�W�F�N�g

	bool mInitingObjects;			// �I�u�W�F�N�g�����������Ă��邩�ǂ���
	bool mUpdatingObjects;			// �I�u�W�F�N�g���X�V���Ă��邩�ǂ���
	bool mIsPause = false;			// �|�[�Y��Ԃ�

	Float3 mCameraLookat;			// �J�����̒����_
	Float3 mCameraEye;				// �J�����̈ʒu
	Float3 mCameraUp = { 0,1,0 };	// �J�����̏�����x�N�g��

	class FadeScreen* fade;

public:
	SceneBase();
	virtual ~SceneBase();

	void SetSceneManager(SceneManager* _sm) {
		mpSceneManager = _sm;
	}

	// ���X�g�ɒǉ�
	void AddGameObject(Dix::sp<GameObject> _object);

	// �I�u�W�F�N�g������
	bool Init();
	// �I�u�W�F�N�g�X�V
	void Update();
	// �I�u�W�F�N�g�`��
	void Render();
	// imgui�f�o�b�O
	virtual void ImguiDebug();
	// �I��
	bool Dispose();

	// �e�V�[���̕ύX�㏈��(�V�[�����؂芷���x�ɌĂ΂��)
	virtual void SceneAfter() = 0;
	// �e�V�[���̏�����(�������Ɉ�x�����Ă΂��)
	virtual void SceneInit() = 0;
	// �e�V�[���̍X�V
	virtual void SceneUpdate() = 0;

	void DrawFadeIn();
	void DrawFadeOut();

	void SetIsPause(bool _flg) { mIsPause = _flg; }
	bool GetIsPause() { return mIsPause; }

	void SetCameraLookat(Float3 _f3) { mCameraLookat = _f3; };
	Float3 GetCameraLookat() { return mCameraLookat; };
	void SetCameraEye(Float3 _f3) { mCameraEye = _f3; };
	Float3 GetCameraEye() { return mCameraEye; };

	void AddDrawComponent(class DrawComponentBase* _c);
	void RemoveDrawComponent(class DrawComponentBase* _c);
};