#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
uniform sampler2D bloom;
out vec4 out_Color;

void main(void)
{
    out_Color = 1.0 * texture(texUnit, outTexCoord) + 1.5 * texture(bloom, outTexCoord);
}
