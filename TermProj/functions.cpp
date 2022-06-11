#pragma once

#include "functions.h"


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

	if (gameState == State::Lobby)
	{
		syncedY = -(2 * ypos / SCR_HEIGHT - 1);
		syncedX = (2 * xpos / SCR_WIDTH - 1);

		if(abs( startButtonPosition.x - syncedX) < 0.2f 
			&& abs(startButtonPosition.y - syncedY) < 0.2f)
		{
			buttonSelected= MousePos::StartButton;
		}
		else if (abs(helpButtonPosition.x - syncedX) < 0.2f
			&& abs(helpButtonPosition.y - syncedY)< 0.2f)
		{
			buttonSelected = MousePos::HelpButton;
		}
		else if (abs(exittButtonPosition.x - syncedX) < 0.2f
			&& abs(exittButtonPosition.y - syncedY) < 0.2f)
		{
			buttonSelected = MousePos::ExitButton;
		}
		else
		{
			buttonSelected = MousePos::Idle;
		}
		
	}
	else if (gameState == State::LobbyHelp)
	{
		syncedY = -(2 * ypos / SCR_HEIGHT - 1);
		syncedX = (2 * xpos / SCR_WIDTH - 1);
		if (abs(backButtonPosition.x - syncedX) < 0.2f
			&& abs(backButtonPosition.y - syncedY) < 0.2f)
		{
			buttonSelected = MousePos::BackButton;
		}
		else
		{
			buttonSelected = MousePos::Idle;
		}
	}

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

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

int GetCardOnPlayer(float x, float z)
{
	float r = pow(0.06, 2); // Card�� Select �� ����
	for (int i = 0; i < 7; i++)
	{
		if (pow(x - card_mid[i].x, 2) + pow(z - card_mid[i].z, 2) < r)
		{
			return i;
		}
	}
	return 7;
}

/// <summary>
/// ������ �� ����(0, 0)�� �������� ĳ������ ��ǥ z, x�� a = 0.21, b = 0.37 �� Ÿ�� �ȿ� �ִ� ��� true �ƴ� ��� false
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


/// <summary>
/// Drawing Card
/// </summary>
/// <param name="num">card number</param>
/// <param name="status">0 : stop 0, 1 : up 360, 2 : down 360, 3 : up 180, 4 : down 180, 5 : stop 180 </param>
/// <returns>model matrix(glm::mat4)</returns>
glm::mat4 DrawCard(int num, int status, float &angle)
{
	glm::mat4 model(1.0f);
	
	switch (status)
	{
	case 0:
		model = glm::translate(model, card[num]);
		model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(cAngle[num]), glm::vec3(0, 0, 1));
		//model = glm::rotate(model, glm::radians(hAngle[0]), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(hAngle[1]), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(hAngle[2]), glm::vec3(0, 0, 1));
		break;
	case 1:
		card[num].y += 0.003;
		angle += 3.f;
		model = glm::translate(model, card[num]);
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(cAngle[num]), glm::vec3(0, 0, 1));
		break;
	case 2:
		card[num].y -= 0.003;
		angle += 3.f;
		model = glm::translate(model, card[num]);
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(cAngle[num]), glm::vec3(0, 0, 1));
		break;
	case 3:
		card[num].y += 0.003;
		angle += 1.5f;
		rotateAngleZ[num] += (cAngle[num] * (-1)) / 60.f;
		model = glm::translate(model, card[num]);
		model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotateAngleZ[num]), glm::vec3(0, 0, 1));
		break;
	case 4:
		card[num].y -= 0.002;
		angle += 1.5f;
		rotateAngleZ[num] += (cAngle[num] * (-1)) / 60.f;
		model = glm::translate(model, card[num]);
		model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotateAngleZ[num]), glm::vec3(0, 0, 1));
		break;
	case 5:
		model = glm::translate(model, card[num]);
		model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(180.f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotateAngleZ[num]), glm::vec3(0, 0, 1));
		break;
	default:
		break;
	}
	
	model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
	return model;
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


#pragma endregion

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
		player.x += Tx;
		player.z += Tz;
		camera.Position.x  += Tx;
		camera.Position.z += Tz;
		if (!PositionCheck()) {
			player.x -= Tx;
			player.z -= Tz;
			camera.Position.x -= Tx;
			camera.Position.z -= Tz;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		player.x -= Tx;
		player.z -= Tz;
		camera.Position.x -= Tx;
		camera.Position.z -= Tz;
		if (!PositionCheck()) {
			player.x += Tx;
			player.z += Tz;
			camera.Position.x += Tx;
			camera.Position.z += Tz;
		}
	}


	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		pAngle += 1;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		pAngle -= 1;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		player[1] += speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		player[1] -= speed;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		switch (buttonSelected)
		{
		case MousePos::Idle:
			break;
		case MousePos::StartButton:
			gameState = State::InGame;
			break;
		case MousePos::HelpButton:
			gameState = State::LobbyHelp;
			break;
		case MousePos::BackButton:
			gameState = State::Lobby;
			break;
		case MousePos::ExitButton:
				glfwSetWindowShouldClose(window, true);
			break;
		default:
			break;
		}
	}
}

int initText() {


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
	std::string font_name = FileSystem::getPath("resources/fonts/Antonio-Regular.ttf");
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


}

void renderUI()
{
	 Shader textShader("shader/text.vs", "shader/text.fs");


}


void load2D(Vertices vertex, unsigned int& VBO, unsigned int& VAO, unsigned int& EBO)
{

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	switch (vertex)
	{
	case Vertices::Background:
		glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertices), backgroundVertices, GL_STATIC_DRAW);
		break;
	case Vertices::Start:
		glBufferData(GL_ARRAY_BUFFER, sizeof(startButtonVertices), startButtonVertices, GL_STATIC_DRAW);
		break;
	case Vertices::Help:
		glBufferData(GL_ARRAY_BUFFER, sizeof(helpButtonVertices), helpButtonVertices, GL_STATIC_DRAW);
		break;
	case Vertices::Exit:
		glBufferData(GL_ARRAY_BUFFER, sizeof(exitButtonVertices), exitButtonVertices, GL_STATIC_DRAW);
		break;
	case Vertices::Back:
		glBufferData(GL_ARRAY_BUFFER, sizeof(backButtonVertices), backButtonVertices, GL_STATIC_DRAW);
		break;
	default:
		break;
	}
	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxIndices), boxIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void loadSkybox(Vertices vertex, unsigned int& VBO, unsigned int& VAO)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
#pragma region Text
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

