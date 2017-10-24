#version 150 core

uniform sampler2D textureMap;

in vec3 position;
in vec2 texCoord;

out vec4 fragColor;

void main()
{
	fragColor = texture(textureMap, texCoord);
	// fragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
