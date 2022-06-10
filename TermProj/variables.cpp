#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "variables.h"
#include <learnopengl/filesystem.h>
#include "root_directory.h"

const char* logl_root = "C:/repo/TermProj";

Model jCard;
Model eCard;
Model table;

Shader textShader;
Shader tableShader;
Shader jCardShader;
Shader eCardShader;
Shader cubeShader;

// transition
glm::vec3 player = glm::vec3(0, 0.182, 0);
glm::vec3 card[7] = { glm::vec3(0.32, 0.145, 0.095), glm::vec3(0.255, 0.145, -0.029), glm::vec3(0.137, 0.145, -0.11), glm::vec3(0, 0.145, -0.142),
					 glm::vec3(-0.137, 0.145, -0.11), glm::vec3(-0.255, 0.145, -0.029), glm::vec3(-0.32, 0.145, 0.095) };
float pAngle = 0.0;
float cAngle[7] = { -75, -50, -25, 0, 25, 50, 75 }; // �ʱ� rotate ���� �� ��
float rotateAngleX[7] = { -90.f, -90.f, -90.f, -90.f, -90.f, -90.f, -90.f }; // Card Animation�� ����ϴ� angleX ��
float rotateAngleY[7] = { 0.f };
float rotateAngleZ[7] = { -75, -50, -25, 0, 25, 50, 75 }; // Card Animation�� ����ϴ� angleZ ��
float speed = 0.001;
int jokerIndex = 0;
int selected[7] = { 0, };
int selectCard = 0;
int heart = 3;

// debug
float hAngle[3] = { 0, };


// settings
const unsigned int SCR_WIDTH = 1980;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.5f, -0.5f), glm::vec3(0.f, 1.f, 0.f), 90.f, -35.f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

 std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;
unsigned int cubeVAO, cubeVBO;
unsigned int cubeTexture;

State gameState = State::Lobby;

float backgroundVertices[] = {
	// positions          // colors           // texture coords
	 1.0f,  1.0f, 0.1f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	 1.0f, -1.0f, 0.1f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-1.0f, -1.0f, 0.1f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-1.0f,  1.0f, 0.1f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};


unsigned int boxIndices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

float startButtonVertices[] = {
	// positions          // colors           // texture coords
	 -0.5f,  -0.5f, 0.1f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	 -0.5f, -0.7f, 0.1f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.7f, -0.7f, 0.09f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-0.7f,  -0.5f, 0.09f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

extern float cubeVertices[] = {
	// positions          // texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
