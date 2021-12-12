#pragma once

namespace Easing
{

	static float QuintIn(float t, float totaltime, float min, float max)
	{
		max -= min;
		t /= totaltime;
		return max * t * t * t * t * t + min;
	}

	static float Linear(float t, float totaltime, float min, float max)
	{
		return (max - min) * t / totaltime + min;
	}
}