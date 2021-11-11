#include	<assimp/cimport.h>
#include	<assimp\postprocess.h>
#include	<assimp/cimport.h>
#include	<assimp\scene.h>

#include	"assimpscene.h"

bool AssimpScene::Init(std::string filename) {

	mScene = aiImportFile(filename.c_str(),
						  aiProcess_ConvertToLeftHanded |
						  aiProcessPreset_TargetRealtime_MaxQuality);

	if (mScene == nullptr)
	{
		return false;
	}

	return true;
}

// �I������
void AssimpScene::Exit() {
	aiReleaseImport(mScene);
}

// �V�[���擾
const aiScene* AssimpScene::GetScene() const {
	return mScene;
}

// �A�j���[�V�����������Ă��邩
bool AssimpScene::HasAnimation() {
	return mScene->HasAnimations();
}