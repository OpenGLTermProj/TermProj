#pragma once 
#define _CRT_SECURE_NO_WARNINGS

#pragma once
#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <ft2build.h>
#include FT_FREETYPE_H



#include "variables.h"

extern glm::mat4 DrawCard(int num, int status, float &angle);
extern void framebuffer_size_callback(GLFWwindow* window, int width, int height);
extern void mouse_callback(GLFWwindow* window, double xpos, double ypos);
extern void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
extern void processInput(GLFWwindow* window);

extern unsigned int loadTexture(char const* path);
extern bool PositionCheck();


#pragma region Text
/// <summary>
/// Rendering text
/// </summary>
/// <param name="shader"></param>
/// <param name="text"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="scale"></param>
/// <param name="color"></param>
extern void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

template<typename ... Args>
extern std::string string_format(const std::string& format, Args ... args);
#pragma endregion

extern int initText();

extern void renderUI();

extern void load2D(Vertices vertex, unsigned int& VBO, unsigned int& VAO, unsigned int& EBO);

extern void loadSkybox(Vertices vertex, unsigned int& VBO, unsigned int& VAO);

extern unsigned int loadCubemap(vector<std::string> faces);

#endif // !FUNCTIONS_H
