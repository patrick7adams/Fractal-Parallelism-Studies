#version 330 core
layout (location = 0) in vec2 initPos;
layout (location = 1) in vec3 initColor;
out vec3 tempColor;

void main() {
	gl_Position = vec4(initPos.xy, 0.0, 1.0);
	tempColor = initColor;
}