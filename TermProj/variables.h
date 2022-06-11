#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/camera.h>
#include <learnopengl/shader.h>
#include <learnopengl/model.h>
#include <iostream>
#include <map>

#ifndef VARIABLES_H
#define VARIABLES_H


extern enum State:short
{
	Lobby =0,
	InGame,
	End

};

extern enum Vertices : short 
{
	Background=0,
	Start,
	Help,
	Exit
};

extern State gameState;

extern Shader textShader;
extern Shader textShader;
extern Shader tableShader;
extern Shader jCardShader;
extern Shader eCardShader;
extern Shader cubeShader;
extern Shader lobbyBackgroundShader;

extern Model table;
extern Model eCard;
extern Model jCard;

// transition
extern glm::vec3 player;
extern glm::vec3 card[7];
extern float rotateAngleX[7];
extern float rotateAngleY[7];
extern float rotateAngleZ[7];
extern float pAngle;
extern float cAngle[7];
extern float speed ;
extern int jokerIndex;
extern int selectCard ;
extern int selected[7];
extern int heart;

// debug
extern float hAngle[3];

extern unsigned int cubeTexture;

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
extern float backgroundVertices[32];
extern float startButtonVertices[32];
extern float helpButtonVertices[32];
extern float exitButtonVertices[32];
extern unsigned int boxIndices[6];

extern unsigned int backgroundVBO, backgroundVAO, backgroundEBO;
extern unsigned int startButtonVBO, startButtonVAO, startButtonEBO;
extern unsigned int helpButtonVBO, helpButtonVAO, helpButtonEBO;
extern unsigned int exitButtonVBO, exitButtonVAO, exitButtonEBO;
extern unsigned int cubeVAO, cubeVBO, cubeEBO;



#endif
