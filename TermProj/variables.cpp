#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "variables.h"
#include <learnopengl/filesystem.h>
#include "root_directory.h"

const char* logl_root = "C:/repo/TermProj";

State gameState = State::Lobby;
MousePos buttonSelected = MousePos::Idle;
GameSequence sequence = GameSequence::Ready;
float sequenceStartTime = 0.0f;
float fireworkDelta = 5.0f;


Shader textShader;
Shader tableShader;
Shader jCardShader;
Shader eCardShader;
Shader cubeShader;

// transition
glm::vec3 jokerModel = glm::vec3(0.0f, -0.2f, 0.5f);
glm::vec3 characterModel = glm::vec3(0.0f, -0.2f, 0.5f);
glm::vec3 jokerFacing = glm::vec3(0,0,0);
glm::vec3 room = glm::vec3(0.0, 1.3, 0);
glm::vec3 joker = glm::vec3(0.0f, -0.15f, 0.3f);

glm::vec3 player = glm::vec3(0, 0.182, 0);
glm::vec3 card[7] = { glm::vec3(0.32, 0.145, 0.095), glm::vec3(0.255, 0.145, -0.029), glm::vec3(0.137, 0.145, -0.11), glm::vec3(0, 0.145, -0.142),
					 glm::vec3(-0.137, 0.145, -0.11), glm::vec3(-0.255, 0.145, -0.029), glm::vec3(-0.32, 0.145, 0.095) }; // card 그릴 때 사용하는 좌표
glm::vec3 card_mid[7] = { glm::vec3(0.307, 0, 0.099), glm::vec3(0.247, 0, -0.022), glm::vec3(0.133, 0, -0.101), glm::vec3(0, 0, -0.101),
							glm::vec3(-0.133, 0, -0.101), glm::vec3(-0.247, 0, -0.022), glm::vec3(-0.307, 0, 0.099) }; // card 중앙 좌표
float pAngle = 0.0;
float cAngle[7] = { -75, -50, -25, 0, 25, 50, 75 }; // �ʱ� rotate ���� �� ��
float rotateAngleX[7] = { -90.f, -90.f, -90.f, -90.f, -90.f, -90.f, -90.f }; // Card Animation�� ����ϴ� angleX ��
float rotateAngleY[7] = { 0.f };
float rotateAngleZ[7] = { -75, -50, -25, 0, 25, 50, 75 };
float jokerFacingAngle =180;// Card Animation�� ����ϴ� angleZ ��
float baseSpeed = 0.005;
float currentSpeed;
bool isCardGround[7] = { false, };
bool isGround = true;
int jokerIndex = 0;
int selected[7] = { 0, };
int selectCard = 3;
int heart = 3;
bool jokerCardFound =false;
// debug
float hAngle[3] = { 0, };
int stage = 1;


// settings
const unsigned int SCR_WIDTH = 1366;
const unsigned int SCR_HEIGHT = 768;


float syncedY;
float syncedX;

// camera
Camera camera(glm::vec3(0.0f, 0.5f, -0.5f), glm::vec3(0.f, 1.f, 1.f), 90.f, -35.f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

 std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;
unsigned int cubeVAO, cubeVBO, cubeEBO;
unsigned int cubeTexture;

vector<std::string> faces;

AnimationState jokerAnimation = AnimationState::AniIdle;
AnimationState playerAnimation = AnimationState::AniIdle;
float jokerAnimationLastTime =0.0f;
float playerAnimationLastTime = 0.0f;

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
	 -0.8f,  -0.4f, 0.08f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	 -0.8f, -0.8f, 0.08f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.4f, -0.8f, 0.08f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-0.4f,  -0.4f, 0.08f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

float helpButtonVertices[] = {
	// positions          // colors           // texture coords
	 -0.2f,  -0.4f, 0.08f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	 -0.2f, -0.8f, 0.08f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	0.2f, -0.8f, 0.08f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	0.2f,  -0.4f, 0.08f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

float backButtonVertices[] = {
	// positions          // colors           // texture coords
   -0.8f,  0.4f, 0.08f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
   -0.8f,  0.8f, 0.08f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
   -0.4f,  0.8f, 0.08f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
   -0.4f,  0.4f, 0.08f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};


float exitButtonVertices[] = {
	// positions          // colors           // texture coords
	 0.4f,  -0.4f, 0.08f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	 0.4f, -0.8f, 0.08f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	0.8f, -0.8f, 0.08f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	0.8f,  -0.4f, 0.08f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

float cubeVertices[] = {
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

float skyboxVertices[] = {
	// positions          
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,

	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f
};

glm::vec2 startButtonPosition= glm::vec2(-0.6f, -0.6f);
glm::vec2 helpButtonPosition= glm::vec2(0.0f, -0.6f);
glm::vec2 exittButtonPosition= glm::vec2(0.6f, -0.6f);
glm::vec2 backButtonPosition = glm::vec2(-0.6f,0.6f);

unsigned int backgroundVBO=NULL, backgroundVAO=NULL, backgroundEBO=NULL;
unsigned int startButtonVBO = NULL, startButtonVAO = NULL, startButtonEBO = NULL;
unsigned int helpButtonVBO = NULL, helpButtonVAO = NULL, helpButtonEBO = NULL;
unsigned int exitButtonVBO = NULL, exitButtonVAO = NULL, exitButtonEBO = NULL;
unsigned int backButtonVBO = NULL, backButtonVAO = NULL, backButtonEBO = NULL;
unsigned int skyboxVBO = NULL, skyboxVAO = NULL;

std::vector<particles> particle = {};