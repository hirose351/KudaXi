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

	void Init() override;
	void Update() override;
	void Render() override;
	void imguidebug() override;
	void Dispose() override;
};