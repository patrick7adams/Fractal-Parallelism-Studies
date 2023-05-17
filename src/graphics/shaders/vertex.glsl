 #version 330 core
 layout (location = 0) in vec2 vertices;
 layout (location = 1) in vec3 colors;
 out vec4 new_colors;
 
 void main() {
    gl_Position = vec4(vertices, 0.0, 1.0);
    new_colors = vec4(colors, 1.0);
}