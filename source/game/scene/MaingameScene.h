#pragma once
#include	"scene_base.h"
#include	"../../system/dx11/dx11mathutil.h"
#include	"../../system/util/dixsmartptr.h"
#include	<list>

using Microsoft::WRL::ComPtr;

//// ���
//enum class GAMESTATE
//{
//	SETUP,	// ����
//	PLAY,	// �v���C��
//	PAUSE,	// ���f
//	OVER,	// �Q�[���I�[�o�[
//	CLEAR,	// �N���A
//	SELECT,	// �Z���N�g
//};

class MaingameScene : public SceneBase
{
private:
	//std::list<Dix::sp<class GameObject>> mObjList;

public:
	MaingameScene();
	~MaingameScene();

	void SceneInit() override;
	void SceneUpdate() override;
	void SceneRender() override;
	/*void ImguiDebug() override;*/
	bool Dispose() override;
};