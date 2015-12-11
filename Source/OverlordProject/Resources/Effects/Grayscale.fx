//=============================================================================
// Shader uses position and texture
//=============================================================================
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Mirror;
    AddressV = Mirror;
};

// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
};

// Create Rasterizer State (Backface culling) 
RasterizerState BackCulling 
{ 
	CullMode = BACK; 
};

Texture2D gTexture;

struct VS_INPUT_STRUCT
{
	float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

struct PS_INPUT_STRUCT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

PS_INPUT_STRUCT VS( VS_INPUT_STRUCT input )
{
	PS_INPUT_STRUCT output = (PS_INPUT_STRUCT)0;
	// Set the Position
	output.Pos = float4(input.Pos, 1.f);
	// Set the Text coord
	output.Tex = input.Tex;

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel XMeshShader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT_STRUCT input): SV_Target
{
    // Step 1: sample the texture
	float4 sample = gTexture.Sample(samLinear, input.Tex);
	// Step 2: calculate the mean value
	float meanValue = (sample.r + sample.g + sample.b) / 3;
	// Step 3: return the color
    return float4( meanValue, meanValue, meanValue, 1.0f );
}

technique10 Grayscale
{
    pass P0
    {          
        // Set states
		SetRasterizerState(BackCulling);
		SetDepthStencilState(EnableDepth, 0);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

