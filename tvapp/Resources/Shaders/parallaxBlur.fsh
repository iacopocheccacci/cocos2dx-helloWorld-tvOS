#ifdef GL_ES
precision mediump float;
#endif

const float BLUR_RADIUS = 10.0;
const float SAMPLE_STEP = 1.25;

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 resolution;
uniform float blurRadius;
uniform float sampleNum;

vec4 blur(vec2);

void main(void)
{
	vec4 col = blur(v_texCoord);
	gl_FragColor = col * v_fragmentColor;
}

vec4 blur(vec2 p)
{
    vec4 col = vec4(0);
    vec4 tempCol = vec4(0);
    vec2 unit = 1.0 / resolution.xy;
    
    float rgbCount = 0.0;
    float alphaCount = 0.0;
    
    for(float x = - 10.0; x < - 7.5; x += SAMPLE_STEP)
    {
        for(float y = -BLUR_RADIUS; y < BLUR_RADIUS; y += SAMPLE_STEP)
        {
            float weight = (BLUR_RADIUS - abs(x)) * (BLUR_RADIUS - abs(y));
            tempCol = texture2D(CC_Texture0, p + vec2(x * unit.x, y * unit.y));
            col += tempCol * weight;
            rgbCount += weight;
            alphaCount += weight;
        }
    }
    for(float x = - 7.5; x < - 5.0; x += SAMPLE_STEP)
    {
        for(float y = -BLUR_RADIUS; y < BLUR_RADIUS; y += SAMPLE_STEP)
        {
            float weight = (BLUR_RADIUS - abs(x)) * (BLUR_RADIUS - abs(y));
            tempCol = texture2D(CC_Texture0, p + vec2(x * unit.x, y * unit.y));
            col += tempCol * weight;
            rgbCount += weight;
            alphaCount += weight;
        }
    }
    for(float x = - 5.0; x < - 2.5; x += SAMPLE_STEP)
    {
        for(float y = -BLUR_RADIUS; y < BLUR_RADIUS; y += SAMPLE_STEP)
        {
            float weight = (BLUR_RADIUS - abs(x)) * (BLUR_RADIUS - abs(y));
            tempCol = texture2D(CC_Texture0, p + vec2(x * unit.x, y * unit.y));
            col += tempCol * weight;
            rgbCount += weight;
            alphaCount += weight;
        }
    }
    for(float x = - 2.5; x < 0.0; x += SAMPLE_STEP)
    {
        for(float y = -BLUR_RADIUS; y < BLUR_RADIUS; y += SAMPLE_STEP)
        {
            float weight = (BLUR_RADIUS - abs(x)) * (BLUR_RADIUS - abs(y));
            tempCol = texture2D(CC_Texture0, p + vec2(x * unit.x, y * unit.y));
            col += tempCol * weight;
            rgbCount += weight;
            alphaCount += weight;
        }
    }
    for(float x = 0.0; x < 2.5; x += SAMPLE_STEP)
    {
        for(float y = -BLUR_RADIUS; y < BLUR_RADIUS; y += SAMPLE_STEP)
        {
            float weight = (BLUR_RADIUS - abs(x)) * (BLUR_RADIUS - abs(y));
            tempCol = texture2D(CC_Texture0, p + vec2(x * unit.x, y * unit.y));
            col += tempCol * weight;
            rgbCount += weight;
            alphaCount += weight;
        }
    }
    for(float x = 2.5; x < 5.0; x += SAMPLE_STEP)
    {
        for(float y = -BLUR_RADIUS; y < BLUR_RADIUS; y += SAMPLE_STEP)
        {
            float weight = (BLUR_RADIUS - abs(x)) * (BLUR_RADIUS - abs(y));
            tempCol = texture2D(CC_Texture0, p + vec2(x * unit.x, y * unit.y));
            col += tempCol * weight;
            rgbCount += weight;
            alphaCount += weight;
        }
    }
    for(float x = 5.0; x < 7.5; x += SAMPLE_STEP)
    {
        for(float y = -BLUR_RADIUS; y < BLUR_RADIUS; y += SAMPLE_STEP)
        {
            float weight = (BLUR_RADIUS - abs(x)) * (BLUR_RADIUS - abs(y));
            tempCol = texture2D(CC_Texture0, p + vec2(x * unit.x, y * unit.y));
            col += tempCol * weight;
            rgbCount += weight;
            alphaCount += weight;
        }
    }
    for(float x = 7.5; x < 10.0; x += SAMPLE_STEP)
    {
        for(float y = -BLUR_RADIUS; y < BLUR_RADIUS; y += SAMPLE_STEP)
        {
            float weight = (BLUR_RADIUS - abs(x)) * (BLUR_RADIUS - abs(y));
            tempCol = texture2D(CC_Texture0, p + vec2(x * unit.x, y * unit.y));
            col += tempCol * weight;
            rgbCount += weight;
            alphaCount += weight;
        }
    }
    
    
    return vec4(col.rgb / rgbCount, col.a / alphaCount);
    
    return texture2D(CC_Texture0, p);
}
