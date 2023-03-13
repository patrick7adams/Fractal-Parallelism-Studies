#ifndef RENDER_CPP
#define RENDER_CPP
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void checkError() {
	if (glGetError() != GL_NO_ERROR) {
		std::cout << "An error has been found!!!!!!" << std::endl;
	}
}

// Initializes the window, GLFW, and Glad.
GLFWwindow* initialize_window(int resX, int resY) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(resX, resY, "Mandelbrot Set", NULL, NULL);
	if (window == NULL) {
		std::cout << "Window failed to create" << std::endl;
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(1);
	}
	glViewport(0, 0, resX, resY);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return window;
}
// GL_VERTEX_SHADER for vertex shader, GL_FRAGMENT_SHADER for fragment shader
unsigned int initialize_shader(int shaderType) {
	unsigned int shader;
	std::string filepath;
	std::ifstream file;
	if (shaderType == GL_VERTEX_SHADER) {
		file.open("vertex.glsl");
	}
	else if (shaderType == GL_FRAGMENT_SHADER) {
		file.open("fragment.glsl");
	}
	if (!file) {
		std::cout << "File not opened correctly!" << std::endl;
	}
	
	std::stringstream stream;
	stream << file.rdbuf();
	file.close();
	
	std::string tempStr = stream.str();
	const char* temp = tempStr.c_str();
	checkError();
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &temp, NULL);
	glCompileShader(shader);
	checkError();
	int success = 0;
	char log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, log);
		std::cout << "Shader failed to compile.\n" << log << std::endl;
		glDeleteShader(shader);
	}
	checkError();
	return shader;
}

unsigned int initializeProgram() {
	unsigned int vertexShader = initialize_shader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = initialize_shader(GL_FRAGMENT_SHADER);

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

	return shaderProgram;
}

int render(int iter, int resX, int resY, std::vector<std::vector<int>> iterations) {
	int totalPoints = resX * resY;
	GLFWwindow* window = initialize_window(resX, resY);
	// Vertex Data
	// x, y, r, g, b
	float vertices[] = {
	//  x     y      r     g     b
		1.0, 0.0, 1.0, 0.0, 0.0,
		0.5, 0.25, 1.0, 0.0, 0.0,
		0.0, 0.5, 1.0, 0.0, 0.0
	};

	unsigned int shaderProgram = initializeProgram();
	// VBO and VAO Initialization
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);
	// Wireframe Mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 1, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}
#endif