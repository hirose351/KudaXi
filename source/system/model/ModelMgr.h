#pragma once
#include	<memory>
#include	<wrl/client.h>
#include	<unordered_map>
#include	"CModel.h"
#include	"../util/uncopyable.h"

using Microsoft::WRL::ComPtr;

class ModelMgr :public Uncopyable {
private:
	ModelMgr() {
	}
	std::unordered_map<
		std::string,					// �L�[
		std::unique_ptr<CModel>			// �f�[�^
	> mModelhashmap;

public:
	static ModelMgr& GetInstance() {
		static ModelMgr Instance;
		return Instance;
	}

	virtual ~ModelMgr() {
		Finalize();
	}

	// ���f���̌�n��
	void Finalize();

	// ���f�������[�h����
	bool LoadModel(std::string _modelfilename,
				   std::string _vsfilename,
				   std::string _psfilename,
				   std::string _texfilefolder);

	// ���f���̃|�C���^���擾����
	CModel* GetModelPtr(std::string _key);
};