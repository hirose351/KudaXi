#include "psvscommon.hlsl"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(VS_OUTPUT input) : SV_Target
{
    float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);
    texcol = texcol * input.Color;
    clip(texcol.w - 0.00001f);
    return texcol;
}
