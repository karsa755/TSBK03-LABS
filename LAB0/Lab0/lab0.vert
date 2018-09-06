/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in  vec3  in_Position;
in  vec3  in_Normal;
in  vec2  in_TexCoord;
in float time;

uniform mat4 transX;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 rotY;
out float shade;
out vec3 newColor;
out vec2 frag_texcoord;
void main(void)
{
	frag_texcoord = in_TexCoord;
	vec3 newNormal =  in_Normal;
	vec3 L = normalize(vec3(0.0, 0.0, 1.0));
	vec3 Ia = vec3(1.0, 1.0, 1.0);
	vec3 Ka = vec3(1.0, 0.0, 0.0);

	vec3 Id = vec3(0.8, 0.8, 0.8);
	vec3 Kd = vec3(0.0, 0.0, 1.0);

	vec3 Is = vec3(1.0, 1.0, 1.0);
	vec3 Ks = vec3(1.0, 1.0, 1.0);
	vec3 R = normalize(2*dot(L,normalize(newNormal))*normalize(newNormal) - L);
	vec3 V = normalize(vec3(0.0, 0.0, 1.0));
	float n = 20;
	//shade = (mat3(viewMatrix)*in_Normal); // Fake shading
	newColor =  Id*Kd*(dot(L,normalize(newNormal))) + clamp(Is*Ks*pow(dot(R,V), n), 0, 1);
	gl_Position=projectionMatrix*viewMatrix* vec4(in_Position, 1.0);

}
