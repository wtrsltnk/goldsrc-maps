#version 150

uniform sampler2D tex;

in vec2 f_uv;
out vec4 fragColor;

void main()
{
   fragColor = vec4(1.0, 1.0, 1.0, 1.0); //texture2D(tex, f_uv.st);
}
