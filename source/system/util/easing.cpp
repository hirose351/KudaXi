#include	"easing.h"

float EasingProcess::GetEsingAns(EasingType _type, float t, float totaltime, float min, float max)
{
	switch (_type)
	{
	case EasingProcess::EasingType::eQuadIn:
	{
		return EasingProcess::QuadIn(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eQuadOut:
	{
		return EasingProcess::QuadOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eQuadInOut:
	{
		return  EasingProcess::QuadInOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eCubicIn:
	{
		return EasingProcess::CubicIn(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eCubicOut:
	{
		return  EasingProcess::CubicOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eCubicInOut:
	{
		return EasingProcess::CubicInOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eQuartIn:
	{
		return EasingProcess::QuartIn(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eQuartOut:
	{
		return EasingProcess::QuartOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eQuartInOut:
	{
		return EasingProcess::QuartInOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eQuintIn:
	{
		return EasingProcess::QuintIn(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eQuintOut:
	{
		return EasingProcess::QuintOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eQuintInOut:
	{
		return EasingProcess::QuintInOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eSineIn:
	{
		return EasingProcess::SineIn(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eSineOut:
	{
		return EasingProcess::SineOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eSineInOut:
	{
		return EasingProcess::SineInOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eExpIn:
	{
		return EasingProcess::ExpIn(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eExpOut:
	{
		return EasingProcess::ExpOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eExpInOut:
	{
		return EasingProcess::ExpInOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eCircIn:
	{
		return EasingProcess::CircIn(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eCircOut:
	{
		return EasingProcess::CircOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eCircInOut:
	{
		return EasingProcess::CircInOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eElasticIn:
	{
		return EasingProcess::ElasticIn(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eElasticOut:
	{
		return EasingProcess::ElasticOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eElasticInOut:
	{
		return EasingProcess::ElasticInOut(t, totaltime, min, max);
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
	case EasingProcess::EasingType::eBounceIn:
	{
		return EasingProcess::BounceIn(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eBounceOut:
	{
		return EasingProcess::BounceOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eBounceInOut:
	{
		return EasingProcess::BounceInOut(t, totaltime, min, max);
	}
	break;
	case EasingProcess::EasingType::eLinear:
	{
		return EasingProcess::Linear(t, totaltime, min, max);
	}
	break;
	}
	return 0.0f;
}
