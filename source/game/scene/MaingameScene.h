#pragma once
#include	"IScene.h"
#include	"../../system/dx11/dx11mathutil.h"
#include	"../../system/util/dixsmartptr.h"
#include	<list>

using Microsoft::WRL::ComPtr;

// ���
enum class GAMESTATE
{
	SETUP,	// ����
	PLAY,	// �v���C��
	PAUSE,	// ���f
	OVER,	// �Q�[���I�[�o�[
	CLEAR,	// �N���A
	SELECT,	// �Z���N�g
};

class MaingameScene : public IScene
{
private:
	std::list<Dix::sp<class GameObject>> mObjList;

public:
	MaingameScene();
	~MaingameScene();

	void AddGameObject() override;
	bool Init() override;
	void Update() override;
	void Render() override;
	void ImguiDebug() override;
	bool Dispose() override;
};