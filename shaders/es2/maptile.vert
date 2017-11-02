#version 100

uniform mat4 modelViewProjection;

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec4 vertexTangent;
attribute vec2 vertexTexCoord;

varying vec2 texCoord;

void main()
{
	// Calculate vertex position in screen space
	gl_Position = modelViewProjection * vec4(vertexPosition, 1.0);

	// Pass texture coordinate to fragment shader
	// Value will be automatically interpolated to fragments inside polygon faces
	texCoord = vertexTexCoord;
}
