#pragma once

// #include "../../lib/headers/glad.h"
#include "glfw3.h"
#include "../util/constants.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* initialize_window();
unsigned int initialize_shader(int shaderType);
unsigned int initializeProgram();
void initializeRenderer(unsigned int* shaderProgram, unsigned int* VAO, unsigned int* vertex_buffer, unsigned int* color_buffer, GLFWwindow* window);
void bufferData(float* vertices, float* colors, unsigned int vertex_buffer, unsigned int color_buffer);
void save(float* colorData, int iter);
void render(unsigned int VAO, GLFWwindow* window, unsigned int shaderProgram);