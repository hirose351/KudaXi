#include "psvscommon.hlsl"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

Texture2D toonTex : register(t4); // テクスチャ

float4 main(VS_OUTPUT input) : SV_Target
{
    float4 N = input.Normal;
    float4 L = LightDirection;
    N.w = 0.0f; // 法線はベクトルなのでＷの値を０にする。
    L.w = 0.0f; // 光の方向はベクトルなのでＷの値を０にする。

    N = normalize(N);
    L = normalize(L);
    
    //N.x *= input.Pos.x / 0.01f;
    //N.y *= input.Pos.y / 0.01f;
    //N.z *= input.Pos.z / 0.01f;
    
   //ハーフランバート拡散照明によるライティング計算
    float p = dot(N.xyz * -1.0f, L.xyz);
    p = p * 0.5f + 0.5f;
    p = p * p;

    
   //色情報をテクセルのＵ成分とし、トゥーンマップテクスチャーから光の反射率を取得する
    float4 Col = toonTex.Sample(g_SamplerLinear, float2(p, 0.0f));
   
   //色情報を格納する    
    float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);
    texcol.xyz *= Col.xyz;
    return texcol;
}
