#version 430

layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoord;

uniform mat4 mvpMatrix;

layout (binding=0) uniform sampler2D samp;

out vec2 varyingTexCoord;

void main(){
	varyingTexCoord = texCoord;
	gl_Position = mvpMatrix * vec4(position, 1.0);
}