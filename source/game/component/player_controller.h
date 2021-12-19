#pragma once
#include		"../../system/util/dixsmartptr.h"
#include		"../component/component_base.h"
#include		"../gameobject/transform.h"
#include		"../state/state.h"
#include		<map>

class State;
namespace Component {
	class PlayerController :public ComponentBase
	{
	private:
		std::map<int, Dix::sp<State>> mStates;
		int mStatesNum;

	public:
		PlayerController();
		~PlayerController();
		void Awake()override {};
		void Init()override;
		void Update()override;
		void Draw()override;
		void ImguiDraw()override;
		void Uninit()override {};
	};
}
