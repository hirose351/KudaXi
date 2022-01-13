#include "psvscommon.hlsl"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

Texture2D toonTex : register(t4); // �e�N�X�`��

float4 main(VS_OUTPUT input) : SV_Target
{
    float4 N = input.Normal;
    float4 L = LightDirection;
    N.w = 0.0f; // �@���̓x�N�g���Ȃ̂łv�̒l���O�ɂ���B
    L.w = 0.0f; // ���̕����̓x�N�g���Ȃ̂łv�̒l���O�ɂ���B

    N = normalize(N);
    L = normalize(L);
    
    //N.x *= input.Pos.x / 0.01f;
    //N.y *= input.Pos.y / 0.01f;
    //N.z *= input.Pos.z / 0.01f;
    
   //�n�[�t�����o�[�g�g�U�Ɩ��ɂ�郉�C�e�B���O�v�Z
    float p = dot(N.xyz * -1.0f, L.xyz);
    p = p * 0.5f + 0.5f;
    p = p * p;

    
   //�F�����e�N�Z���̂t�����Ƃ��A�g�D�[���}�b�v�e�N�X�`���[������̔��˗����擾����
    float4 Col = toonTex.Sample(g_SamplerLinear, float2(p, 0.0f));
   
   //�F�����i�[����    
    float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);
    texcol.xyz *= Col.xyz;
    return texcol;
}
