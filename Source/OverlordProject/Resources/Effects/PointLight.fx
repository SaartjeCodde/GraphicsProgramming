//float3 position = float3(0.0f, 0.0f, 0.0f);;
//float range = 100.0f;
//float3 attenuation = float3(0.0f,0.2f,0.0f);
//float4 ambient = float4(0.3f, 0.3f, 0.3f, 1.0f);
//float4 diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);;

float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
// float3 gLightDirection = float3(1, 1, 1);
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);

struct PointLight
{
	float3 dir;
	float3 pos;
	float  range;
	float3 att;
	float4 ambient;
	float4 diffuse;
};

cbuffer cbPerFrame
{
	PointLight light;
};

Texture2D gDiffuseMap;
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};

struct VS_INPUT{
	float3 pos : POSITION;
	float2 TexCoord : TEXCOORD;
	float3 Normal : NORMAL;
};
struct VS_OUTPUT{
	float4 pos : SV_POSITION;
	float4 WorldPos : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
};



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input){

	VS_OUTPUT output;

	output.pos = mul ( float4(input.pos,1.0f), gWorldViewProj );

	// output.WorldPos = mul(input.pos gWorld);
	// Step 1:	convert position into float4 and multiply with matWorldViewProj
	//output.pos = mul ( float4(input.pos,1.0f), gWorldViewProj );
	// Step 2:	rotate the normal: NO TRANSLATION
	//			this is achieved by clipping the 4x4 to a 3x3 matrix, 
	//			thus removing the postion row of the matrix
	output.WorldPos = mul ( float4(input.pos,1.0f), gWorld );
	output.Normal = normalize(mul(input.Normal, (float3x3)gWorld));
	output.TexCoord = input.TexCoord;
	return output;
}



//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET{

	input.Normal = normalize(input.Normal);
	
	float4 diffuseColor = gDiffuseMap.Sample(samLinear,input.TexCoord );
	
		float3 finalColor = float3(0.0f, 0.0f, 0.0f);

	// Vector between light position & pixel
	float3 lightToPixelVec = light.pos - input.WorldPos;

	//// Distance between light pos & pixel pos
	float d = length(lightToPixelVec);

	//// Create the ambient light
	float3 finalAmbient = diffuseColor / 20 * light.ambient;

	//// If pixel is to far, return pixel color with ambient light
	 if (d > light.range)
	 	return float4(finalAmbient, diffuseColor.a);

	lightToPixelVec /= d;

	float howMuchLight = dot(lightToPixelVec, input.Normal);

	if( howMuchLight > 0.0f )
	{	
		//Add light to the finalColor of the pixel
		finalColor += howMuchLight * diffuseColor * light.diffuse;
		
		//Calculate Light's Falloff factor
		finalColor /= light.att[0] + (light.att[1] * d) + (light.att[2] * (d*d));
	}	

	finalColor = saturate(finalColor + finalAmbient);

	//float3 color_rgb= diffuseColor.rgb;
	//float color_a = diffuseColor.a;
	//
	////HalfLambert Diffuse :)
	/// float diffuseStrength = dot(input.Normal, light.pos);
	//diffuseStrength = diffuseStrength * 0.5 + 0.5;
	//diffuseStrength = saturate(diffuseStrength);
	//color_rgb = color_rgb * diffuseStrength;

	return float4( finalColor * 30  , diffuseColor.a );
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(Solid);
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

