#version 430

uniform mat4 mvpMatrix;

layout (binding=0) uniform sampler2D samp;

in vec2 varyingTexCoord;
out vec4 fragColor;

void main(){
	fragColor = texture(samp, varyingTexCoord);
}