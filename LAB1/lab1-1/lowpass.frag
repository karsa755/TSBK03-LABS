#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

struct row {
    float arr[5];
};

void main(void)
{
    //init
    vec2 dims = textureSize(texUnit, 0);
    float dimX = dims.x;
    float dimY = dims.y;
    float curPixelX = outTexCoord.x * dimX;
    float curPixelY = outTexCoord.y*dimY;
    vec4 result = vec4(0.0);
    row weights[5] = row[5](
                            row(float[5](1,4,6,4,1)),
                            row(float[5](4,16,24,16,4)),
                            row(float[5](6,24,36,24,6)),
                            row(float[5](1,4,6,4,1)),
                            row(float[5](4,16,24,16,4)));

    for(int i = -2; i <= 2; ++i)
    {
        for(int j = -2; j <= 2; ++j)
        {
            if(!( (curPixelX + i) > dimX || (curPixelX+i) < 0 || (curPixelY + j) > dimY || (curPixelY+j) < 0  ))
            {
                vec2 texCoords = vec2( (curPixelX+i) / dimX, (curPixelY+i) / dimY);
                result += texture(texUnit, texCoords) * (weights[i+2].arr[j+2] / 256);
            }
        }
    }
    out_Color = result;
}
