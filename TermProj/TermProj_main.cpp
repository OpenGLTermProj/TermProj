#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>
#include <cstdlib>
#include <ctime>

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args);

void DrawHUD();
bool PositionCheck();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);

// transition
glm::vec3 player = glm::vec3(0, 0.182, 0);
float pAngle = 0.0;
float speed = 0.001;
int selectCard = 0;

// settings
const unsigned int SCR_WIDTH = 1366;
const unsigned int SCR_HEIGHT = 768;

// camera
Camera camera(glm::vec3(0.0f, 0.5f, -0.5f), glm::vec3(0.f, 1.f, 0.f), 90.f, -35.f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;
#pragma endregion


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	srand(time(NULL)); // rand 함수 사용을 위한 SEED 값 초기화

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TermProj", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// camera initial
	// --------------

	 // compile and setup the shader
	// ----------------------------
	Shader textShader("text.vs", "text.fs");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
	textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// build and compile shaders
	// -------------------------
	Shader tableShader("table.vs", "table.fs");
	Shader cubeShader("cube.vs", "cube.fs");

	// load models
	// -----------
	Model table(FileSystem::getPath("Data/table/table_final.obj"));

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
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

	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// load textures
	// -------------
	unsigned int cubeTexture = loadTexture(FileSystem::getPath("resources/textures/container.jpg").c_str());

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


#pragma region TEXT
	// FreeType
	// --------
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	// find path to font
	std::string font_name = FileSystem::getPath("resources/fonts/Antonio-Bold.ttf");
	if (font_name.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		return -1;
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// configure VAO/VBO for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
#pragma endregion


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		tableShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		tableShader.setMat4("projection", projection);
		tableShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		tableShader.setMat4("model", model);
		table.Draw(tableShader);

		// cubes
		cubeShader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);

		model = glm::mat4(1.0f);
		model = glm::translate(model, player);
		model = glm::rotate(model, glm::radians(pAngle), glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));

		unsigned int modelLoc = glGetUniformLocation(cubeShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(cubeShader.ID, "view");
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);
		cubeShader.setMat4("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		string debug1 = string_format("Front : %f, %f, %f | Position : %f, %f, %f | Yaw : %f | Pitch : %f", camera.Front[0], camera.Front[1], camera.Front[2],
			camera.Position[0], camera.Position[1], camera.Position[2], camera.Yaw, camera.Pitch);
		string debug2 = string_format("Model Position | %f, %f, %f", player[0], player[1], player[2]);

		RenderText(textShader, debug1, 25.0f, 25.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
		RenderText(textShader, debug2, 25.0f, 50.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
		DrawHUD();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera = Camera(glm::vec3(0.0f, 0.5f, -0.5f), glm::vec3(0.f, 1.f, 0.f), 90.f, -35.f);

	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
		player = glm::vec3(0, 0.182, 0);

	// character move
	float Tx = speed * cos((-pAngle - 90) * 3.141592 / 180);
	float Tz = speed * sin((-pAngle - 90) * 3.141592 / 180);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		player[0] += Tx;
		player[2] += Tz;
		if (!PositionCheck()) {
			player[0] -= Tx;
			player[2] -= Tz;
		}	
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		player[0] -= Tx;
		player[2] -= Tz;
		if (!PositionCheck()) {
			player[0] += Tx;
			player[2] += Tz;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		pAngle -= 1;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		pAngle += 1;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		player[1] += speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		player[1] -= speed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

int GetCardOnPlayer(float x, float z)
{
	float r = pow(0.03, 2); // Card를 Select 할 범위
	float a = 0.3, b = 0.1;
	if (pow(x - a, 2) + pow(z - b, 2) < r) return 0;
	a = 0.24; b = 0.02;
	if (pow(x - a, 2) + pow(z - b, 2) < r) 	return 1;
	a = 0.13; b = -0.1;
	if (pow(x - a, 2) + pow(z - b, 2) < r)	return 2;
	a = 0; b = -0.13;
	if (pow(x - a, 2) + pow(z - b, 2) < r)	return 3;
	a = -0.13; b = -0.1;
	if (pow(x - a, 2) + pow(z - b, 2) < r)	return 4;
	a = -0.24; b = 0.02;
	if (pow(x - a, 2) + pow(z - b, 2) < r)	return 5;
	a = -0.3, b = 0.1;
	if (pow(x - a, 2) + pow(z - b, 2) < r)	return 6;
	return 7;
}

/// <summary>
/// 움직일 때 원점(0, 0)을 기준으로 캐릭터의 좌표 z, x가 a = 0.21, b = 0.37 인 타원 안에 있는 경우 true 아닌 경우 false
/// </summary>
/// <returns></returns>
bool PositionCheck()
{
	auto p = glm::vec3(0, 0.182, 0);
	float a = 0.21, b = 0.37;
	float len = 0;
	float x = abs(player[0]);
	float z = abs(player[2]);

	int card = GetCardOnPlayer(player[0], player[2]);
	if (card != selectCard)
	{
		if (card != 7)
		{
			printf("selection card change %d to %d\n", selectCard, card);
			selectCard = card;
		}
	}

	if (x == 0.0000)
		if (z < a)
			return true;
		else
			return false;
	else if (z == 0.0000)
		if (x < b)
			return true;
		else
			return false;
	else
		len = pow(player[0], 2) / pow(b, 2) + pow(player[2], 2) / pow(a, 2);

	if (len < 1)
		return true;
	else
		return false;

}

void DrawHUD() // Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	//gluOrtho2D(0.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glColor3f(0.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(0.05, 0.05);
	glVertex2f(0.3, 0.05);
	glVertex2f(0.3, 0.15);
	glVertex2f(0.05, 0.15);
	glEnd();
	glEnable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

#pragma region TextFunc
// render line of text
// -------------------
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
	// activate corresponding render state	
	shader.use();
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size <= 0) { throw std::runtime_error("Error during formatting."); }
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
#pragma endregion
