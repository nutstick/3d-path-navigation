#version 150

uniform sampler2D texture;
uniform float minAlpha;
uniform float maxAlpha;
uniform float textureSize;
uniform vec4 color;

in vec2 texCoord;
in float zValue;

out vec4 fragColor;

void main()
{
	fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
