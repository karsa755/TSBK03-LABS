#version 150

//in vec3 in_Color;
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
uniform mat4 matrix;
uniform mat4 ROT1;
uniform mat4 ROT2;

uniform vec3 TRANS1;
uniform vec3 TRANS2;

out vec4 g_color;
const vec3 lightDir = normalize(vec3(0.3, 0.5, 1.0));

// Uppgift 3: Soft-skinning p� GPU
//
// Flytta �ver din implementation av soft skinning fr�n CPU-sidan
// till vertexshadern. Mer info finns p� hemsidan.

void main(void)
{

	mat4 translate0 = mat4(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(TRANS1.x, TRANS1.y, TRANS1.z, 1.0));
	mat4 translate1 = mat4(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(TRANS2.x, TRANS2.y, TRANS2.z, 1.0));
	mat4 translate0Inverse = mat4(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(-TRANS1.x, -TRANS1.y, -TRANS1.z, 1.0));
	mat4 translate1Inverse = mat4(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(-TRANS2.x, -TRANS2.y, -TRANS2.z, 1.0));

	mat4 Mbone1 = translate0 * ROT1;
	mat4 Mbone2 = translate1 * ROT2;
	mat4 M1 =   in_TexCoord.x * (Mbone1 * translate0Inverse) ;
	mat4 M2 =   in_TexCoord.y * ( (Mbone1 * Mbone2) * (translate1Inverse * translate0Inverse));
	vec4 V0prim = M1 * vec4(in_Position, 1.0);
	vec4 V1prim = M2 * vec4(in_Position, 1.0);

	// transformera resultatet med ModelView- och Projection-matriserna
	//gl_Position = (matrix) * vec4(V0prim + V1prim);
	gl_Position = matrix * vec4(in_Position, 1.0);

	// s�tt r�d+gr�n f�rgkanal till vertex Weights
	vec4 color = vec4(in_TexCoord.x, in_TexCoord.y, 0.0, 1.0);

	// L�gg p� en enkel ljuss�ttning p� vertexarna
	float intensity = dot(in_Normal, lightDir);
	color.xyz *= intensity;

	g_color = color;
}
