struct VSOutput
{
    float4 Pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

cbuffer ConstantBufferWorld : register(b0)
{
    matrix World;
}

cbuffer ConstantBufferView : register(b1)
{
    matrix View;
}

cbuffer ConstantBufferProjection : register(b2)
{
    matrix Projection;
}

cbuffer _cb0 : register(b3)
{
    float r, g, b, a;
    float u0, v0, u3, v3;
};

VSOutput vs_main(
	float4 Pos : SV_POSITION,
    uint index : SV_VertexID
)
{
    VSOutput output;
    
    output.Pos = mul(Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    
    switch (index)
    {
        case 0:
            output.uv.x = u0;
            output.uv.y = v0;
            break;
        case 1:
            output.uv.x = u3;
            output.uv.y = v0;
            break;
        case 2:
            output.uv.x = u0;
            output.uv.y = v3;
            break;
        case 3:
            output.uv.x = u3;
            output.uv.y = v3;
            break;
    }
    
    return output;
}