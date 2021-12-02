#include	"model_component.h"
#include	"../../system/dx11/Shader.h"
#include	"../../system/dx11/CDirectxGraphics.h"
#include	"../../system/dx11/DX11Settransform.h"

#include	<windows.h>

using namespace Component;

void ModelComponent::Draw()
{
	_mpModel->Draw(mOwner->GetTransform()->worldMtx);
}

void Component::ModelComponent::ImguiDraw()
{

}

void ModelComponent::Uninit()
{
	_mpModel = nullptr;
}