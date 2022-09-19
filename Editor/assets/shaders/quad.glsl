// 2D Quad Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

struct VertexOutput
{
	vec4 Color;
};

layout(location = 0) out VertexOutput vOutput;

uniform mat4 uViewProjection;

void main()
{
	vColor = aColor;

	gl_Position = vec4(aPosition, 1.0);
	//gl_Position = uViewProjection * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec4 Color;
};

layout(location = 0) in VertexOutput Input;

void main()
{
	vec4 texColor = Input.Color;

	if (texColor.a == 0)
		discard;

	o_Color = texColor;
	o_EntityID = v_EntityID;
}
