#pragma once
#include	<string>
#include	<assimp/cimport.h>

class AssimpScene {
private:
	const aiScene* mScene = nullptr;			// assimp scene
public:
	// ��������
	bool Init(std::string filename);
	// �I������
	void Exit();

	// �V�[���擾
	const aiScene* GetScene() const;

	// �A�j���[�V�����������Ă��邩
	bool HasAnimation();
};