#version 150

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexTangent;
in vec2 vertexTexCoord;

out vec2 texCoord;

uniform mat4 modelMatrix;

void main()
{
	texCoord = vertexTexCoord;
	gl_Position = modelMatrix * vec4(1.0);
}
