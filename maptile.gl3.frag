#version 150

#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

in sampler2D texture;
in vec2 v_texcoord;

void main()
{
	// Set fragment color from texture
	gl_FragColor = texture2D(texture, v_texcoord);
}
