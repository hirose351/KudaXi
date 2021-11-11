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
	// Zバッファ有効化
	CDirectXGraphics::GetInstance()->TurnOnZBuffer();

	// Zバッファ無効化
	CDirectXGraphics::GetInstance()->TurnOffZbuffer();

}

void MaingameScene::imguidebug()
{
}

void MaingameScene::Dispose()
{
}