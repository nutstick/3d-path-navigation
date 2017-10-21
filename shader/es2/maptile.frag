#version 100

uniform highp sampler2D texture;

// varying highp vec2 texCoord;

void main()
{
	// Set fragment color from texture
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
