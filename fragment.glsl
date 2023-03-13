#version 330 core
in vec3 tempColor;
out vec4 FragColor;

void main() {
	FragColor = vec4(tempColor.xyz, 1.0);
}