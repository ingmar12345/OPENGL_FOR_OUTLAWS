#version 420 core

in float polyDepth;
out vec4 color;

uniform float depthOffset;
uniform float depthRange;

void main(void)
{
	// diepte component
	float depthValue  = (polyDepth / depthRange) + depthOffset;
	vec3 result = vec3(depthValue, depthValue, depthValue);

	color = vec4(result, 1.0f);
}