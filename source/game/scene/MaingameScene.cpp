#include	"MaingameScene.h"

MaingameScene::MaingameScene()
{
}

MaingameScene::~MaingameScene()
{
	Dispose();
}

void MaingameScene::Init()
{
}

void MaingameScene::Update()
{
}

void MaingameScene::Render()
{
	// Z�o�b�t�@�L����
	CDirectXGraphics::GetInstance()->TurnOnZBuffer();

	// Z�o�b�t�@������
	CDirectXGraphics::GetInstance()->TurnOffZbuffer();

}

void MaingameScene::imguidebug()
{
}

void MaingameScene::Dispose()
{
}