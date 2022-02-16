#include	"model_manager.h"

void ModelMgr::Finalize()
{
	for (auto itr = mModelhashmap.begin();
		 itr != mModelhashmap.end();
		 ++itr)
	{

		(itr->second)->Uninit();
	}
}

bool ModelMgr::LoadModel(std::string _modelfilename, std::string _vsfilename, std::string _psfilename, std::string _texfilefolder)
{
	// ���݂��邩���m���߂�
	auto it = mModelhashmap.find(_modelfilename);
	// ���݂��Ă��Ȃ���Γǂݍ���
	if (it == mModelhashmap.end())
	{
		std::unique_ptr<CModel>	p;

		p = std::make_unique<CModel>();

		// ���f���ǂݍ���
		bool sts = p->Init(
			_modelfilename.c_str(),
			_vsfilename.c_str(),
			_psfilename.c_str(),
			_texfilefolder.c_str());
		if (!sts)
		{
			return false;
		}

		// unordered_map�R���e�i�Ɋi�[
		mModelhashmap[_modelfilename].swap(p);
	}
	return true;
}

CModel * ModelMgr::GetModelPtr(std::string _key)
{
	// ���݂��邩���m���߂�
	auto it = mModelhashmap.find(_key);
	if (it == mModelhashmap.end())
	{
		return nullptr;
	}
	return mModelhashmap[_key].get();
}
