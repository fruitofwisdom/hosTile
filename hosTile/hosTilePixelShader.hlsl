Texture2D inputTexture : register(t0);
SamplerState inputSampler : register(s0);

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 textureColor;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = inputTexture.Sample(inputSampler, input.tex);

	return textureColor;
}
