#pragma once
#include		"../../system/util/uncopyable.h"
#include		<vector>

class DrawComponentBase;

class DrawManager
{
private:
	std::vector<DrawComponentBase*> mComponentList;	// 


public:
	void AddDrawComponent(DrawComponentBase* _component);
	void RemoveDrawComponent(DrawComponentBase* _component);

	void Draw();
};

