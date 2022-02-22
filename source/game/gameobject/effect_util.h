#pragma once
#include	"../../system/dx11/dx11_util.h"

struct PARTICLE {
	int		uv;						// UV�ԍ�
	int		lifetime;				// ��������
	float	cameradistance;			// ����
	DirectX::XMFLOAT3	pos;		// �ʒu
	DirectX::XMFLOAT3	velocity;	// ���x
	float	gravity;				// �d��
	float	height;					// �n��
	float	attenuation;			// ����
};

// �召��r�p�̊֐��I�u�W�F�N�g
struct ParticleLess {
	bool operator()(const PARTICLE& _a, const PARTICLE& _b) const noexcept {
		// �L�[�Ƃ��Ĕ�r�������v�f��񋓂���
		return _a.cameradistance > _b.cameradistance;
	}
};