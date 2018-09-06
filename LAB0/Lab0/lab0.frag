/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in float shade;
in vec3 newColor;

out vec4 out_Color;
uniform sampler2D exampletexture;
in vec2 frag_texcoord;
void main(void)
{

  out_Color=texture(exampletexture,frag_texcoord) + vec4(newColor, 1.0);
}
