#include "psvscommon.hlsl"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float2 resolution = float2(1280, 720);
static const float GRID = 4.;
static const float PI = 3.14159265;

float random(in float2 st)
{
    return frac(sin(dot(st.xy, float2(12.9898, 78.233))) * 43758.5453123);
}

float circle(in float2 _st, in float2 _circle, in float _radius)
{
    float2 d = _circle - _st;
    return step(dot(d, d), _radius * _radius);
}

float poly(in float2 _st, in float2 _poly, in float _rad)
{
    // polygon is 4 points on a circle
    // _poly is coordinates of origin of polygon
    float4 _angles = float4(random(_poly + float2(.1, .1)), random(_poly + float2(.2, .2)),
                    random(_poly + float2(.3, .3)), random(_poly + float2(.4, .4)));
    float _total = _angles[0] + _angles[1] + _angles[2] + _angles[3];
    _angles *= 2. * PI / _total;
    
    _angles[1] += _angles[0];
    _angles[2] += _angles[1];
    _angles[3] += _angles[2];
    
    float4 _a2 = float4(0., 0., 0., 0.);
    
    float2 _p0 = float2(_poly.x + _rad * cos(_angles[0]), _poly.y + _rad * sin(_angles[0]));
    float2 _p1 = float2(_poly.x + _rad * cos(_angles[1]), _poly.y + _rad * sin(_angles[1]));
    float2 _p2 = float2(_poly.x + _rad * cos(_angles[2]), _poly.y + _rad * sin(_angles[2]));
    float2 _p3 = float2(_poly.x + _rad * cos(_angles[3]), _poly.y + _rad * sin(_angles[3]));
    
    _a2[0] = atan2(_p0.y - _st.y, _p0.x - _st.x); // cant assign these in a loop?
    _a2[1] = atan2(_p1.y - _st.y, _p1.x - _st.x);
    _a2[2] = atan2(_p2.y - _st.y, _p2.x - _st.x);
    _a2[3] = atan2(_p3.y - _st.y, _p3.x - _st.x);
    
    float4 _a3 = _a2.gbar;
    
    float _ret = 1.;
    
    for (int _i = 0; _i < 4; _i++)
    {
        float _aDiff = _a3[_i] - _a2[_i];
        
        _aDiff = fmod(_aDiff + 2. * PI, 2. * PI);
        
        // if angle difference > 180 degrees then return 0
        _ret *= step(_aDiff, PI);
    }
    
    
    return _ret;
}

float4 main(VS_OUTPUT input) : SV_Target
{
  
    float2 fragCoord = input.Pos.xy;

    float2 st = fragCoord / 8.;
    
    float shade = 0.;
    
    float2 i = floor(st);
    float2 f = frac(st);
    
    float3 col = float3(0., 0., 0.);
    
    for (float xx = -GRID; xx <= GRID; xx++)
    {
        for (float yy = -GRID; yy <= GRID; yy++)
        {
        
            float2 i2 = i + float2(xx, yy);
            
            float merge = random(i2 + float2(-1000., -1000.));
            float3 _col = float3(0., merge, 1.);
            
            float brightOff = random(i2 + float2(-2000., -2000.)) * 2. * PI;
            float brightness = (1. + sin(time + brightOff)) * .5;
            brightness = brightness * brightness * brightness;
            _col = (1. - brightness) * _col + brightness * float3(1., 1., 1.);
            
            float rad = .5 + (GRID - .5) * random(i2);
            
            float _a = .5 * poly(st, i2 + float2(.5, .5), rad);
            col = (1. - _a) * col + _a * _col;
        }
    }
    
    
    return float4(col, 1.);
        
        
        
}
