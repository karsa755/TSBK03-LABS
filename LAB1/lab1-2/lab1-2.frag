#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 out_Normal;
in vec3 Ps;
in vec3 Pt;
in vec3 pixPos;  // Needed for specular reflections
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
    vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates

	// Calculate gradients here
	float offset = 1.0 / 256.0; // texture size, same in both directions

  float Bs = (texture(texUnit, vec2(outTexCoord.x + offset, outTexCoord.y )) - texture(texUnit, outTexCoord)).x;
  float Bt = (texture(texUnit, vec2(outTexCoord.x ,outTexCoord.y + offset)) - texture(texUnit, outTexCoord)).y;
  vec3 normal = normalize(vec3(Bs, Bt, 1.0));
  //vec3 normal = normalize(out_Normal + Bs*Ps + Bt*Pt);
  //vec3 normal = normalize(out_Normal);
  mat3 Mvt = mat3(Ps, Pt, out_Normal);
  vec3 preOutColor = vec3( dot(normal, Mvt*light));


    //out_Color = vec4( dot(normal, light));
    out_Color =  vec4(preOutColor, 1.0);
}
