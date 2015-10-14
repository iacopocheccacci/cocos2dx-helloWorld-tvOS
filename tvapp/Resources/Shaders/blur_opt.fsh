#ifdef GL_ES
precision mediump float;
#endif

#ifdef GL_ES
varying lowp vec4 v_color;
varying highp vec2 blurCoordinates[5];
#else
varying vec4 v_color;
varying vec2 blurCoordinates[5];
#endif

void main()
{
#ifdef GL_ES
    lowp vec4 sum = vec4(0.0);
#else
    vec4 sum = vec4(0.0);
#endif
    sum += texture2D(CC_Texture0, blurCoordinates[0]) * 0.204164;
    sum += texture2D(CC_Texture0, blurCoordinates[1]) * 0.304005;
    sum += texture2D(CC_Texture0, blurCoordinates[2]) * 0.304005;
    sum += texture2D(CC_Texture0, blurCoordinates[3]) * 0.093913;
    sum += texture2D(CC_Texture0, blurCoordinates[4]) * 0.093913;
    gl_FragColor = sum * v_color;
}