// 2D Quad Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aTexIndex;
layout(location = 4) in float aTilingFactor;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};

layout(location = 0) out VertexOutput vOutput;

uniform mat4 uViewProjection;

void main()
{
	vOutput.Color = aColor;
	vOutput.TexCoord = aTexCoord;
	vOutput.TexIndex = aTexIndex;
	vOutput.TilingFactor = aTilingFactor;

	gl_Position = uViewProjection * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 oColor;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};

layout(location = 0) in VertexOutput Input;

layout(binding = 0) uniform sampler2D uTextures[32];

void main()
{
	vec4 texColor = Input.Color;

	switch(int(Input.TexIndex))
	{
		case  0: texColor *= texture(uTextures[ 0], Input.TexCoord * Input.TilingFactor); break;
		case  1: texColor *= texture(uTextures[ 1], Input.TexCoord * Input.TilingFactor); break;
		case  2: texColor *= texture(uTextures[ 2], Input.TexCoord * Input.TilingFactor); break;
		case  3: texColor *= texture(uTextures[ 3], Input.TexCoord * Input.TilingFactor); break;
		case  4: texColor *= texture(uTextures[ 4], Input.TexCoord * Input.TilingFactor); break;
		case  5: texColor *= texture(uTextures[ 5], Input.TexCoord * Input.TilingFactor); break;
		case  6: texColor *= texture(uTextures[ 6], Input.TexCoord * Input.TilingFactor); break;
		case  7: texColor *= texture(uTextures[ 7], Input.TexCoord * Input.TilingFactor); break;
		case  8: texColor *= texture(uTextures[ 8], Input.TexCoord * Input.TilingFactor); break;
		case  9: texColor *= texture(uTextures[ 9], Input.TexCoord * Input.TilingFactor); break;
		case 10: texColor *= texture(uTextures[10], Input.TexCoord * Input.TilingFactor); break;
		case 11: texColor *= texture(uTextures[11], Input.TexCoord * Input.TilingFactor); break;
		case 12: texColor *= texture(uTextures[12], Input.TexCoord * Input.TilingFactor); break;
		case 13: texColor *= texture(uTextures[13], Input.TexCoord * Input.TilingFactor); break;
		case 14: texColor *= texture(uTextures[14], Input.TexCoord * Input.TilingFactor); break;
		case 15: texColor *= texture(uTextures[15], Input.TexCoord * Input.TilingFactor); break;
		case 16: texColor *= texture(uTextures[16], Input.TexCoord * Input.TilingFactor); break;
		case 17: texColor *= texture(uTextures[17], Input.TexCoord * Input.TilingFactor); break;
		case 18: texColor *= texture(uTextures[18], Input.TexCoord * Input.TilingFactor); break;
		case 19: texColor *= texture(uTextures[19], Input.TexCoord * Input.TilingFactor); break;
		case 20: texColor *= texture(uTextures[20], Input.TexCoord * Input.TilingFactor); break;
		case 21: texColor *= texture(uTextures[21], Input.TexCoord * Input.TilingFactor); break;
		case 22: texColor *= texture(uTextures[22], Input.TexCoord * Input.TilingFactor); break;
		case 23: texColor *= texture(uTextures[23], Input.TexCoord * Input.TilingFactor); break;
		case 24: texColor *= texture(uTextures[24], Input.TexCoord * Input.TilingFactor); break;
		case 25: texColor *= texture(uTextures[25], Input.TexCoord * Input.TilingFactor); break;
		case 26: texColor *= texture(uTextures[26], Input.TexCoord * Input.TilingFactor); break;
		case 27: texColor *= texture(uTextures[27], Input.TexCoord * Input.TilingFactor); break;
		case 28: texColor *= texture(uTextures[28], Input.TexCoord * Input.TilingFactor); break;
		case 29: texColor *= texture(uTextures[29], Input.TexCoord * Input.TilingFactor); break;
		case 30: texColor *= texture(uTextures[30], Input.TexCoord * Input.TilingFactor); break;
		case 31: texColor *= texture(uTextures[31], Input.TexCoord * Input.TilingFactor); break;
	}

	if (texColor.a == 0)
		discard;

	oColor = texColor;
}
