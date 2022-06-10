#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/camera.h>
#include <learnopengl/shader.h>
#include <iostream>
#include <map>

#ifndef VARIABLES_H
#define VARIABLES_H


// transition
extern glm::vec3 player;
extern glm::vec3 card[7];
extern float pAngle  ;
extern float cAngle[7];
extern float speed ;
extern int jokerIndex;
extern int selectCard ;

// settings
extern const unsigned int SCR_WIDTH ;
extern const unsigned int SCR_HEIGHT;

// camera
extern Camera camera;
extern float lastX;
extern float lastY;
extern bool firstMouse;

// timing
extern float currentFrame;
extern float deltaTime;
extern float lastFrame;

#pragma region Text
/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};
extern std::map<GLchar, Character> Characters;
extern unsigned int VAO, VBO;
#pragma endregion

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
extern float cubeVertices[180];

#endif