#pragma once
#include <cassert>
#include "../dx11/dx11mathutil.h"
#include <stdio.h>
#include <iostream>

namespace Easing
{
	static void Assert(float totaltime)
	{
		assert(totaltime > 0);
		//std::cout << "totaltime‚ª0ˆÈ‰º‚Å‚·";
	}
	static float QuadIn(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t /= totaltime;
		return max * t * t + min;
	}

	static float QuadOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t /= totaltime;
		return -max * t * (t - 2) + min;
	}

	static float QuadInOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t /= totaltime / 2;
		if (t < 1) return max / 2 * t * t + min;

		t = t - 1;
		return -max / 2 * (t * (t - 2) - 1) + min;
	}

	static float CubicIn(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t /= totaltime;
		return max * t * t * t + min;
	}

	static float CubicOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t = t / totaltime - 1;
		return max * (t * t * t + 1) + min;
	}

	static float CubicInOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t /= totaltime / 2;
		if (t < 1) return max / 2 * t * t * t + min;

		t = t - 2;
		return max / 2 * (t * t * t + 2) + min;
	}

	static float QuartIn(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t /= totaltime;
		return max * t * t * t * t + min;
	}

	static float QuartOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t = t / totaltime - 1;
		return -max * (t * t * t * t - 1) + min;
	}

	static float QuartInOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t /= totaltime / 2;
		if (t < 1) return max / 2 * t * t * t * t + min;

		t = t - 2;
		return -max / 2 * (t * t * t * t - 2) + min;
	}

	static float QuintIn(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t /= totaltime;
		return max * t * t * t * t * t + min;
	}

	static float QuintOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t = t / totaltime - 1;
		return max * (t * t * t * t * t + 1) + min;
	}

	static float QuintInOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t /= totaltime / 2;
		if (t < 1) return max / 2 * t * t * t * t * t + min;

		t = t - 2;
		return max / 2 * (t * t * t * t * t + 2) + min;
	}

	static float SineIn(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		return -max * cos(t * (DirectX::XM_PI * 90 / 180) / totaltime) + max + min;
	}

	static float SineOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		return max * sin(t * (DirectX::XM_PI * 90 / 180) / totaltime) + min;
	}

	static float SineInOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		return -max / 2 * (cos(t * DirectX::XM_PI / totaltime) - 1) + min;
	}

	static float ExpIn(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		return t == 0.0f ? min : static_cast<float>(max * pow(2, 10 * (t / totaltime - 1)) + min);
	}

	static float ExpOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		return t == totaltime ? max + min : static_cast<float>(max * (-pow(2, -10 * t / totaltime) + 1) + min);
	}

	static float ExpInOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		if (t == 0.0f) return min;
		if (t == totaltime) return max;
		max -= min;
		t /= totaltime / 2;

		if (t < 1) return static_cast<float>(max / 2 * pow(2, 10 * (t - 1)) + min);

		t = t - 1;
		return static_cast<float>(max / 2 * (-pow(2, -10 * t) + 2) + min);
	}

	static float CircIn(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t /= totaltime;
		return -max * (sqrt(1 - t * t) - 1) + min;
	}

	static float CircOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t = t / totaltime - 1;
		return max * sqrt(1 - t * t) + min;
	}

	static float CircInOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t /= totaltime / 2;
		if (t < 1) return -max / 2 * (sqrt(1 - t * t) - 1) + min;

		t = t - 2;
		return max / 2 * (sqrt(1 - t * t) + 1) + min;
	}

	static float ElasticIn(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);

		max -= min;
		t /= totaltime;

		float s = 1.70158f;
		float p = totaltime * 0.3f;
		float a = max;

		if (t == 0) return min;
		if (t == 1) return min + max;

		if (a < abs(max))
		{
			a = max;
			s = p / 4;
		}
		else
		{
			s = p / (2 * DirectX::XM_PI) * asin(max / a);
		}

		t = t - 1;
		return static_cast<float>(-(a * pow(2, 10 * t) * sin((t * totaltime - s) * (2 * DirectX::XM_PI) / p)) + min);
	}

	static float ElasticOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);

		max -= min;
		t /= totaltime;

		float s = 1.70158f;
		float p = totaltime * 0.3f; ;
		float a = max;

		if (t == 0) return min;
		if (t == 1) return min + max;

		if (a < abs(max))
		{
			a = max;
			s = p / 4;
		}
		else
		{
			s = p / (2 * DirectX::XM_PI) * asin(max / a);
		}

		return static_cast<float>(a * pow(2, -10 * t) * sin((t * totaltime - s) * (2 * DirectX::XM_PI) / p) + max + min);
	}

	static float ElasticInOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);

		max -= min;
		t /= totaltime / 2;

		float s = 1.70158f;
		float p = totaltime * (0.3f * 1.5f);
		float a = max;

		if (t == 0) return min;
		if (t == 2) return min + max;

		if (a < abs(max))
		{
			a = max;
			s = p / 4;
		}
		else
		{
			s = p / (2 * DirectX::XM_PI) * asin(max / a);
		}


		if (t < 1)
		{
			return static_cast<float>(-0.5f * (a * pow(2, 10 * (t -= 1)) * sin((t * totaltime - s) * (2 * DirectX::XM_PI) / p)) + min);
		}

		t = t - 1;
		return static_cast<float> (a * pow(2, -10 * t) * sin((t * totaltime - s) * (2 * DirectX::XM_PI) / p) * 0.5f + max + min);
	}

	static float BackIn(float t, float totaltime, float min, float max, float s)
	{
		Assert(totaltime);
		max -= min;
		t /= totaltime;
		return max * t * t * ((s + 1) * t - s) + min;
	}

	static float BackOut(float t, float totaltime, float min, float max, float s)
	{
		Assert(totaltime);
		max -= min;
		t = t / totaltime - 1;
		return max * (t * t * ((s + 1) * t + s) + 1) + min;
	}

	static float BackInOut(float t, float totaltime, float min, float max, float s)
	{
		Assert(totaltime);
		max -= min;
		s *= 1.525f;
		t /= totaltime / 2;
		if (t < 1) return max / 2 * (t * t * ((s + 1) * t - s)) + min;

		t = t - 2;
		return max / 2 * (t * t * ((s + 1) * t + s) + 2) + min;
	}

	static float BounceOut(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		max -= min;
		t /= totaltime;

		if (t < 1.0f / 2.75f)
		{
			return max * (7.5625f * t * t) + min;
		}
		else if (t < 2.0f / 2.75f)
		{
			t -= 1.5f / 2.75f;
			return max * (7.5625f * t * t + 0.75f) + min;
		}
		else if (t < 2.5f / 2.75f)
		{
			t -= 2.25f / 2.75f;
			return max * (7.5625f * t * t + 0.9375f) + min;
		}
		else
		{
			t -= 2.625f / 2.75f;
			return max * (7.5625f * t * t + 0.984375f) + min;
		}
	}

	static float BounceIn(float t, float totaltime, float min, float max)
	{
		max -= min;
		return max - BounceOut(totaltime - t, totaltime, 0, max) + min;
	}

	static float BounceInOut(float t, float totaltime, float min, float max)
	{
		if (t < totaltime / 2)
		{
			return BounceIn(t * 2, totaltime, 0, max - min) * 0.5f + min;
		}
		else
		{
			return BounceOut(t * 2 - totaltime, totaltime, 0, max - min) * 0.5f + min + (max - min) * 0.5f;
		}
	}

	static float Linear(float t, float totaltime, float min, float max)
	{
		Assert(totaltime);
		//return (max - min) * t / totaltime + min;
	}
}