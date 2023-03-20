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

const int resX = 1000;
const int resY = 1000;
const int totalPoints = resX * resY;
const int iterations = 1000;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void checkError() {
	if (glGetError() != GL_NO_ERROR) {
		std::cout << "An error has been found!!!!!!" << std::endl;
	}
}

// Initializes the window, GLFW, and Glad.
GLFWwindow* initialize_window() {
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
	glBindAttribLocation(shaderProgram, 0, "vertices");
	glBindAttribLocation(shaderProgram, 0, "colors");
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

void getColor(float *color, int iter) {
	for (int i = 0; i < 3; i++) {
		if (iter == iterations) {
			color[i] = 1.0;
		}
		else {
			color[i] = 0.0;
		}
	}
}

int render(std::vector<std::vector<int>> vec) {
	GLFWwindow* window = initialize_window();
	int lenVertices = totalPoints * 2, lenColors = totalPoints * 3;
	float* vertices = new float[lenVertices];
	float* colors = new float[lenColors];
	float* color = new float[3];

	for (int i = 0; i < resX; i++) {
		for (int k = 0; k < resY; k++) {
			vertices[(i * resX + k)*2] = ((float)(i*2-resX))/(resX);
			vertices[(i * resX + k)*2+1] = ((float)(k*2-resY))/(resY);
			getColor(color, vec[i][k]);
			colors[(i * resX + k) * 3] = color[0];
			colors[(i * resX + k) * 3+1] = color[1];
			colors[(i * resX + k) * 3+2] = color[2];
		}
	}
	delete[] color;
	unsigned int shaderProgram = initializeProgram();
	// VBO and VAO Initialization

	unsigned int VAO, vertex_buffer, color_buffer;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &color_buffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, lenVertices * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	delete[] vertices;

	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, lenColors * sizeof(float), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	delete[] colors;
	// Wireframe Mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(shaderProgram);
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, resX*resY*2);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}
#endif