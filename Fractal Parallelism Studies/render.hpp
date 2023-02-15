#ifndef RENDER_HPP
#define RENDER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"gl_position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;"
"void main() {FragColor = vec4(1.0, 1.0, 1.0, 1.0);}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int render() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1000, 1000, "Mandelbrot Set", NULL, NULL);
	if (window == NULL) {
		std::cout << "Window failed to create" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 1;
	}
	glViewport(0, 0, 1000, 1000);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Vertex Data
	float vertices[]{
			0.5, 0.5, 0.0,
			0.5, -0.5, 0.0,
			-0.5, -0.5, 0.0,
			-0.5, 0.5, 0.0
	};
	unsigned int indices[]{
		0, 1, 2,
		0, 3, 2
	};
    // Vertex Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int successVert;
	char logVert[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVert);
	if (!successVert) {
		glGetShaderInfoLog(vertexShader, 512, NULL, logVert);
		std::cout << "Vertex shader failed to compile.\n" << logVert << std::endl;
	}
	// Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int successFrag;
	char logFrag[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFrag);
	if (!successFrag) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, logFrag);
		std::cout << "Fragment shader failed to compile.\n" << logFrag << std::endl;
	}
	// Program Initialization
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int successProg;
	char logProg[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successProg);
	if (!successProg) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, logProg);
		std::cout << "Program failed to compile.\n" << logProg << std::endl;
	}
	// Shader Deletion
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	// VBO and VAO Initialization
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Wireframe Mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0, 0.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}

#endif