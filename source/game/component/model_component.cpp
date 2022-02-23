#include	"model_component.h"
#include	"../../system/dx11/Shader.h"
#include	"../../system/dx11/directx_graphics.h"
#include	"../../system/dx11/dx11_settransform.h"
#include	<windows.h>

using namespace Component;

Model::Model()
{
	SetName("Model");
}

void Model::Draw()
{
	mpModel->Draw(mWorldMtx);
}

void Model::ImguiDrawComponent()
{

}

void Model::Uninit()
{
	mpModel = nullptr;
}