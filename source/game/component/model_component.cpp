#include	"model_component.h"
#include	"../../system/dx11/Shader.h"
#include	"../../system/dx11/CDirectxGraphics.h"
#include	"../../system/dx11/DX11Settransform.h"

#include	<windows.h>

using namespace Component;

void Model::Draw()
{
	mpModel->Draw(mOwner->GetTransform()->worldMtx);
}

void Component::Model::ImguiDraw()
{

}

void Model::Uninit()
{
	mpModel = nullptr;
}