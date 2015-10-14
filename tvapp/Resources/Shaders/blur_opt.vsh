attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

#ifdef GL_ES
varying lowp vec4 v_color;
#else
varying vec4 v_color;
#endif

uniform float texelWidthOffset;
uniform float texelHeightOffset;

varying vec2 blurCoordinates[5];

void main() {
    v_color = a_color;
    vec2 singleStepOffset = vec2(texelWidthOffset, texelHeightOffset);
    blurCoordinates[0] = a_texCoord.xy;
    blurCoordinates[1] = a_texCoord.xy + singleStepOffset * 1.407333;
    blurCoordinates[2] = a_texCoord.xy - singleStepOffset * 1.407333;
    blurCoordinates[3] = a_texCoord.xy + singleStepOffset * 3.294215;
    blurCoordinates[4] = a_texCoord.xy - singleStepOffset * 3.294215;
    gl_Position = CC_PMatrix * a_position;
}
