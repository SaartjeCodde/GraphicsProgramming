Texture2D gTexture;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Clamp;
    AddressV = Clamp;
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

struct VS_INPUT_STRUCT
{
    float3 Pos : POSITION;
	float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT_STRUCT
{
    float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};

PS_INPUT_STRUCT VS(VS_INPUT_STRUCT input)
{
	PS_INPUT_STRUCT output = (PS_INPUT_STRUCT)0;
	// Set the Position
	output.Pos = float4(input.Pos, 1.f);
	// Set the Text coord
	output.TexCoord = input.TexCoord;

	return output;
}

float4 PS(PS_INPUT_STRUCT input):SV_TARGET
{
	float4 sample = gTexture.Sample(samLinear, input.TexCoord);
	float3 color = sample.rgb;
	
	// Step 1: find the dimensions of the texture (the texture has a method for that)
	float sceneWidth = 0.f;
	float sceneHeight = 0.f;
	
	gTexture.GetDimensions(sceneWidth, sceneHeight);

	float2 sceneDimensions = float2(sceneWidth, sceneHeight);
	
	// Step 2: calculate dx and dy
	float dx = 1.f / sceneWidth;
	float dy = 1.f / sceneHeight;
	
	// Step 3: Create a double for loop (5 iterations each)
	//		Inside the look, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
	//		Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
	for (int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			float2 offset = float2(0, 0);
			offset.x = ((j - 2) * 2) * dx;
			offset.y = ((i - 2) * 2) * dy;
			color += gTexture.Sample(samLinear, input.TexCoord + offset);
		}
	}

	// Step 4: Divide the final color by the number of passes (in this case 5*5)
	color = color / (5 * 5);

	// Step 5: return the final color
	return float4(color, sample.a);	
}

technique11 Blur
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