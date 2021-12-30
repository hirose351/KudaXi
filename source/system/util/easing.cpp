#include	"easing.h"

float Easing::GetEsingAns(EasingType _type, float t, float totaltime, float min, float max)
{
	switch (_type)
	{
	case Easing::EasingType::eQuadIn:
	{
		return Easing::QuadIn(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eQuadOut:
	{
		return Easing::QuadOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eQuadInOut:
	{
		return  Easing::QuadInOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eCubicIn:
	{
		return Easing::CubicIn(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eCubicOut:
	{
		return  Easing::CubicOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eCubicInOut:
	{
		return Easing::CubicInOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eQuartIn:
	{
		return Easing::QuartIn(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eQuartOut:
	{
		return Easing::QuartOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eQuartInOut:
	{
		return Easing::QuartInOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eQuintIn:
	{
		return Easing::QuintIn(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eQuintOut:
	{
		return Easing::QuintOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eQuintInOut:
	{
		return Easing::QuintInOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eSineIn:
	{
		return Easing::SineIn(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eSineOut:
	{
		return Easing::SineOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eSineInOut:
	{
		return Easing::SineInOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eExpIn:
	{
		return Easing::ExpIn(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eExpOut:
	{
		return Easing::ExpOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eExpInOut:
	{
		return Easing::ExpInOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eCircIn:
	{
		return Easing::CircIn(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eCircOut:
	{
		return Easing::CircOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eCircInOut:
	{
		return Easing::CircInOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eElasticIn:
	{
		return Easing::ElasticIn(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eElasticOut:
	{
		return Easing::ElasticOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eElasticInOut:
	{
		return Easing::ElasticInOut(t, totaltime, min, max);
	}
	break;
	//case Easing::EasingType::EBackIn:
	//{
	//	ans.x = Easing::BackIn(mTime, node.totalTime, node.startValue.x, node.endValue.x);
	//	ans.y = Easing::BackIn(mTime, node.totalTime, node.startValue.y, node.endValue.y);

	//}
	//break;
	//case Easing::EasingType::EBackOut:
	//{
	//	ans.x = Easing::BackOut(mTime, node.totalTime, node.startValue.x, node.endValue.x);
	//	ans.y = Easing::BackOut(mTime, node.totalTime, node.startValue.y, node.endValue.y);

	//}
	//break;
	//case Easing::EasingType::EBackInOut:
	//{
	//	ans.x = Easing::BackInOut(mTime, node.totalTime, node.startValue.x, node.endValue.x);
	//	ans.y = Easing::BackInOut(mTime, node.totalTime, node.startValue.y, node.endValue.y);

	//}
	//break;
	case Easing::EasingType::eBounceIn:
	{
		return Easing::BounceIn(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eBounceOut:
	{
		return Easing::BounceOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eBounceInOut:
	{
		return Easing::BounceInOut(t, totaltime, min, max);
	}
	break;
	case Easing::EasingType::eLinear:
	{
		return Easing::Linear(t, totaltime, min, max);
	}
	break;
	}
	return 0.0f;
}
