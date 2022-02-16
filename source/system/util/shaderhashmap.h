#pragma once
#include	<unordered_map>
#include	<d3d11.h>
#include	<string>
#include	"../dx11/DX11util.h"
#include	"../dx11/Shader.h"

class ShaderHashmap {
private:
	ShaderHashmap() {}
	std::unordered_map<std::string, ID3D11VertexShader*> mVsHashMap;
	std::unordered_map<std::string, ID3D11PixelShader*> mPsHashMap;
	std::unordered_map<std::string, ID3D11InputLayout*> mLayoutHashMap;

public:
	ShaderHashmap(const ShaderHashmap&) = delete;
	ShaderHashmap& operator=(const ShaderHashmap&) = delete;
	ShaderHashmap(ShaderHashmap&&) = delete;
	ShaderHashmap& operator=(ShaderHashmap&&) = delete;

	static ShaderHashmap* GetInstance()
	{
		static ShaderHashmap instance;
		return &instance;
	}

	// �s�N�Z���V�F�[�_�[�Z�b�g
	bool SetPixelShader(std::string filename)
	{
		ID3D11PixelShader* psh;

		// �f�o�C�X�擾
		ID3D11Device* device;
		device = GetDX11Device();

		// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
		bool sts = CreatePixelShader(device,
									 filename.c_str(),
									 "main",
									 "ps_5_0",
									 &psh);

		if (!sts)
		{
			MessageBox(nullptr, "CreatePixel Shader error", "error", MB_OK);
			return false;
		}

		// unordered_map�R���e�i�Ɋi�[
		mPsHashMap[filename] = psh;

		return true;
	}

	// ���_�V�F�[�_�[�Z�b�g
	bool SetVertexShader(std::string filename,
						 D3D11_INPUT_ELEMENT_DESC* layout,
						 unsigned int numElements)
	{

		ID3D11VertexShader* vsh;
		ID3D11InputLayout*  vlayout;

		// �f�o�C�X�擾
		ID3D11Device* device;
		device = GetDX11Device();

		ID3DBlob* pBlob = nullptr;

		void* ShaderObject;
		size_t	ShaderObjectSize;

		// �t�@�C���̊g���q�ɍ��킹�ăR���p�C��
		HRESULT hr = CompileShader(
			filename.c_str(),
			"main",
			"vs_5_0",
			&ShaderObject,
			ShaderObjectSize,
			&pBlob);
		if (FAILED(hr))
		{
			if (pBlob)pBlob->Release();
			return false;
		}

		// ���_�V�F�[�_�[�𐶐�
		hr = device->CreateVertexShader(
			ShaderObject,
			ShaderObjectSize,
			nullptr,
			&vsh);
		if (FAILED(hr))
		{
			if (pBlob)pBlob->Release();
			return false;
		}

		// ���_�f�[�^��`����
		hr = device->CreateInputLayout(
			layout,
			numElements,
			ShaderObject,
			ShaderObjectSize,
			&vlayout);
		if (FAILED(hr))
		{
			MessageBox(nullptr, "CreateInputLayout error", "error", MB_OK);
			pBlob->Release();
			return false;
		}

		// unordered_map�R���e�i�Ɋi�[
		mVsHashMap[filename] = vsh;
		mLayoutHashMap[filename] = vlayout;

		// ���������
		if (pBlob)pBlob->Release();

		return true;
	}

	// ���_�V�F�[�_�[�擾
	ID3D11VertexShader* GetVertexShader(std::string filenamekey)
	{
		// ���݂��邩���m���߂�
		auto it = mVsHashMap.find(filenamekey);
		if (it == mVsHashMap.end())
		{
			return nullptr;
		}
		return mVsHashMap[filenamekey];
	}

	// �s�N�Z���V�F�[�_�[�擾
	ID3D11PixelShader* GetPixelShader(std::string filenamekey)
	{
		// ���݂��邩���m���߂�
		auto it = mPsHashMap.find(filenamekey);
		if (it == mPsHashMap.end())
		{
			return nullptr;
		}
		return mPsHashMap[filenamekey];
	}

	// ���_���C�A�E�g�擾
	ID3D11InputLayout* GetVertexLayout(std::string filenamekey)
	{
		// ���݂��邩���m���߂�
		auto it = mLayoutHashMap.find(filenamekey);
		if (it == mLayoutHashMap.end())
		{
			return nullptr;
		}
		return mLayoutHashMap[filenamekey];
	}

	// �I������
	void Exit() {

		// ���
		for (auto data : mVsHashMap)
		{
			data.second->Release();
		}

		mVsHashMap.clear();

		// ���
		for (auto data : mPsHashMap)
		{
			data.second->Release();
		}

		mPsHashMap.clear();

		// ���
		for (auto data : mLayoutHashMap)
		{
			data.second->Release();
		}

		mLayoutHashMap.clear();
	}
};