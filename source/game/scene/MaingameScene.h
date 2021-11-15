#pragma once
#include	"../../system/dx11/util/dx11mathutil.h"
#include	"../gameobject/skydome.h"
#include	"../manager/SceneManager.h"
#include	"../manager/SelectManager.h"

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


public:
	MaingameScene();
	~MaingameScene();

	bool Init() override;
	void Update() override;
	void Render() override;
	void ImguiDebug() override;
	bool Dispose() override;
};