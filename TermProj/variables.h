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
#define PI 3.141592
#define AVG_DELTATIME 0.02
#define MAX_PARTICLE 100

extern enum State:short
{
	Lobby =0,
	LobbyHelp,
	InGame,
	End

};

extern enum Vertices : short 
{
	Background=0,
	Start,
	Help,
	Back,
	Exit,
	Skybox,
	Cube
};

extern enum MousePos : short
{
	Idle =0,
	StartButton,
	HelpButton,
	BackButton,
	ExitButton
};

enum GameSequence : short {
	Ready=0,
	CountDown,
	Hammer,
	Wait,
	Check,
	Finish,
};

typedef struct {
	bool    active;
	float   life;
	float   fade;
	glm::vec3 color;
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 force;
}particles;


extern particles particle[MAX_PARTICLE];
extern State gameState;
extern MousePos buttonSelected;
extern GameSequence sequence;
extern float sequenceStartTime;

extern Shader textShader;
extern Shader textShader;
extern Shader tableShader;
extern Shader jCardShader;
extern Shader eCardShader;
extern Shader cubeShader;
extern Shader lobbyBackgroundShader;

// transition
extern glm::vec3 jokerFacing;
extern glm::vec3 jokerModel;
extern glm::vec3 characterModel;
extern glm::vec3 room;
extern glm::vec3 joker;
extern glm::vec3 player;
extern glm::vec3 card[7];
extern glm::vec3 card_mid[7];
extern float rotateAngleX[7];
extern float rotateAngleY[7];
extern float rotateAngleZ[7];
extern float pAngle;
extern float cAngle[7];
extern float baseSpeed;
extern float currentSpeed;
extern float jokerFacingAngle;
extern bool isCardGround[7];
extern bool isGround;
extern int jokerIndex;
extern int selectCard ;
extern int selected[7];
extern int heart;
extern bool jokerCardFound;

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

extern vector<std::string> faces;

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
extern float backButtonVertices[32];
extern float skyboxVertices[108];
extern unsigned int boxIndices[6];

extern glm::vec2 startButtonPosition;
extern glm::vec2 helpButtonPosition;
extern glm::vec2 exittButtonPosition;
extern glm::vec2 backButtonPosition;

extern unsigned int backgroundVBO, backgroundVAO, backgroundEBO;
extern unsigned int startButtonVBO, startButtonVAO, startButtonEBO;
extern unsigned int helpButtonVBO, helpButtonVAO, helpButtonEBO;
extern unsigned int exitButtonVBO, exitButtonVAO, exitButtonEBO;
extern unsigned int backButtonVBO, backButtonVAO, backButtonEBO;
extern unsigned int skyboxVBO, skyboxVAO;
extern unsigned int cubeVAO, cubeVBO, cubeEBO;


extern float syncedY;
extern float syncedX;

enum AnimationState : short {
	AniIdle = 0,
	Running,
	Hammering
};

extern AnimationState jokerAnimation;
extern AnimationState playerAnimation;

extern float jokerAnimationLastTime;
extern float playerAnimationLastTime;
#endif
