#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "functions.h"

GLFWwindow* window;
void robby();
void inGame();

int main(int argc, char** argv)
{

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	srand(time(NULL)); // rand �Լ� ����� ���� SEED �� �ʱ�ȭ

    // glfw window creation
    // --------------------

	
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TermProj", NULL, NULL);
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

	// card index set
	jokerIndex = rand() % 7;

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//
	initText();

	// build and compile shaders
// -------------------------
	Model jCard(FileSystem::getPath("Data/card/jokercard.obj"));
	Model eCard(FileSystem::getPath("Data/card/emptycard.obj"));
	Model table(FileSystem::getPath("Data/table/table_final.obj"));
	Model hammer(FileSystem::getPath("Data/hammer/hammer.obj"));

	Shader textShader("shader/text.vs", "shader/text.fs");
	Shader tableShader("shader/table.vs", "shader/table.fs");
	Shader jCardShader("shader/jokercard.vs", "shader/jokercard.fs");
	Shader eCardShader("shader/emptycard.vs", "shader/emptycard.fs");
	Shader cubeShader("shader/cube.vs", "shader/cube.fs");
	Shader lightingShader("shader/light_casters.vs", "shader/light_casters.fs");
	Shader lobbyShader("shader/lobby.vs", "shader/lobby.fs");
	Shader skyboxShader("shader/skybox.vs", "shader/skybox.fs");
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);

	vector<std::string> faces =
	{
		FileSystem::getPath("resources/textures/skybox/right.jpg"),
		FileSystem::getPath("resources/textures/skybox/left.jpg"),
		FileSystem::getPath("resources/textures/skybox/top.jpg"),
		FileSystem::getPath("resources/textures/skybox/bottom.jpg"),
		FileSystem::getPath("resources/textures/skybox/front.jpg"),
		FileSystem::getPath("resources/textures/skybox/back.jpg")
	};

	// load textures
// -------------
	unsigned int cubeTexture = loadTexture(FileSystem::getPath("resources/textures/container.jpg").c_str());
	unsigned int backgroundTexture = loadTexture(FileSystem::getPath("resources/textures/joker_with_letter.jpg").c_str());
	unsigned int lobbyHelpTexture = loadTexture(FileSystem::getPath("resources/textures/jokerHelp.jpg").c_str());
	unsigned int buttonTexture = loadTexture(FileSystem::getPath("resources/textures/button.jpg").c_str());
	unsigned int buttonStartTexture = loadTexture(FileSystem::getPath("resources/textures/startButton.jpg").c_str());
	unsigned int buttonHelpTexture = loadTexture(FileSystem::getPath("resources/textures/helpButton.jpg").c_str());
	unsigned int buttonExitTexture = loadTexture(FileSystem::getPath("resources/textures/exitButton.jpg").c_str());
	unsigned int buttonBackTexture = loadTexture(FileSystem::getPath("resources/textures/backButton.jpg").c_str());
	unsigned int cubemapTexture = loadCubemap(faces);
	

	// load models
	// -----------


	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
	textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// cube VAO
	
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	load2D(Vertices::Start, startButtonVBO, startButtonVAO, startButtonEBO);
	load2D(Vertices::Background, backgroundVBO, backgroundVAO, backgroundEBO);
	load2D(Vertices::Help, helpButtonVBO, helpButtonVAO, helpButtonEBO);
	load2D(Vertices::Exit, exitButtonVBO, exitButtonVAO, exitButtonEBO);
	load2D(Vertices::Back, backButtonVBO, backButtonVAO, backButtonEBO);

	
	//load2D(cubeVertices, cubeVBO, cubeVAO, cubeEBO);


	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

		switch (gameState)
		{
		case State::Lobby:
		{
			
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			lobbyShader.use();
	
			glBindTexture(GL_TEXTURE_2D, backgroundTexture);
			glBindVertexArray(backgroundVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			buttonSelected == MousePos::StartButton ? glBindTexture(GL_TEXTURE_2D, buttonStartTexture) : glBindTexture(GL_TEXTURE_2D, buttonTexture);
			glBindVertexArray(startButtonVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

			buttonSelected == MousePos::HelpButton ? glBindTexture(GL_TEXTURE_2D, buttonHelpTexture) : glBindTexture(GL_TEXTURE_2D, buttonTexture);
			glBindVertexArray(helpButtonVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			buttonSelected == MousePos::ExitButton ? glBindTexture(GL_TEXTURE_2D, buttonExitTexture) : glBindTexture(GL_TEXTURE_2D, buttonTexture);
			glBindVertexArray(exitButtonVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			string debug1 = string_format(" Mouse X : %f  Mouse Y : %f", lastX, lastY);
			string debug2 = string_format("Front : %f, %f, %f | Position : %f, %f, %f | Yaw : %f | Pitch : %f", camera.Front[0], camera.Front[1], camera.Front[2],
				camera.Position[0], camera.Position[1], camera.Position[2], camera.Yaw, camera.Pitch);
			string debug3 = string_format(" Mouse nomal X : %f  Mouse  nomal Y : %f", syncedX, syncedY);
			RenderText(textShader, debug1, 25.0f, 25.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
			RenderText(textShader, debug2, 25.0f, 50.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
			RenderText(textShader, debug3, 25.0f, 75.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));

			break;
		}
		case State::LobbyHelp:
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			lobbyShader.use();
			glBindTexture(GL_TEXTURE_2D, lobbyHelpTexture);
			glBindVertexArray(backgroundVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			buttonSelected == MousePos::BackButton ? glBindTexture(GL_TEXTURE_2D, buttonBackTexture) : glBindTexture(GL_TEXTURE_2D, buttonTexture);
			glBindVertexArray(backButtonVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			string help1 = string_format("1. Find Joker Card");
			string help2 = string_format("2. Flip Cards using Clown's Hammering");
			string help3 = string_format("3. Move Character with W, A, S, D");
			string help4 = string_format("4. Find Joker in 3 times");
			string help5 = string_format("p.s. Hope you good luck");

			RenderText(textShader, help1, 150.0f, 500.0f, 1.25f, glm::vec3(1.0, 0.0f, 0.0f));
			RenderText(textShader, help2, 150.0f, 425.0f, 1.25f, glm::vec3(1.0, 0.0f, 0.0f));
			RenderText(textShader, help3, 150.0f, 350.0f, 1.25f, glm::vec3(1.0, 0.0f, 0.0f));
			RenderText(textShader, help4, 150.0f, 275.0f, 1.25f, glm::vec3(1.0, 0.0f, 0.0f));
			RenderText(textShader, help5, 150.0f, 150.0f, 1.0f, glm::vec3(1.0, 0.0f, 0.0f));

			break;
		}
		case State::InGame:
		{
			lightingShader.use();
			//lightingShader.setVec3("light.position", glm::vec3(card[selectCard].x, 0.5, card[selectCard].z));
			lightingShader.setVec3("light.position", glm::vec3(0, 3, 0));
			lightingShader.setVec3("light.direction", glm::vec3(0, -1, 0));
			lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(90.f)));
			lightingShader.setVec3("viewPos", camera.Position);

			// light properties
			lightingShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
			// we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
			// each environment and lighting type requires some tweaking to get the best out of your environment.
			lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
			lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("light.constant", 1.0f);
			lightingShader.setFloat("light.linear", 0.09f);
			lightingShader.setFloat("light.quadratic", 0.032f);

			// material properties
			lightingShader.setFloat("material.shininess", 32.0f);

			// view/projection transformations
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			lightingShader.setMat4("projection", projection);
			lightingShader.setMat4("view", view);

			// world transformation
			glm::mat4 model = glm::mat4(1.0f);
			lightingShader.setMat4("model", model);

			lightingShader.setMat4("model", model);
			table.Draw(lightingShader);

			for (int i = 0; i < 7; i++)
			{
				if (selected[i] == 0)
				{
					card[i].y = 0.145;
					model = DrawCard(i, 0, rotateAngleX[i]);	// it's a bit too big for our scene, so scale it down
				}
				else if (selected[i] == 2)
				{
					rotateAngleX[i] = -90.f;
					rotateAngleY[i] = 180.f;
					rotateAngleZ[i] = -cAngle[i];
					card[i].y = 0.195;
					model = DrawCard(i, 5, rotateAngleY[i]);
				}
				else if (selected[i] == 1)
				{
					if (i == selectCard)
					{
						if (rotateAngleY[i] < 90.f)
						{
							model = DrawCard(i, 3, rotateAngleY[i]);
						}
						else if (rotateAngleY[i] > 180.f)
						{
							selected[i] = 2;
							rotateAngleX[i] = -90.f;
							rotateAngleY[i] = 180.f;
							rotateAngleZ[i] = -cAngle[i];
							card[i].y = 0.195;
							model = DrawCard(i, 5, rotateAngleY[i]);
						}
						else
						{
							model = DrawCard(i, 4, rotateAngleY[i]);
						}
					}
					else
					{
						if (rotateAngleX[i] < 90.f)
						{
							model = DrawCard(i, 1, rotateAngleX[i]);
						}
						else if (rotateAngleX[i] > 270.f)
						{
							selected[i] = 0;
							rotateAngleX[i] = -90.f;
							card[i].y = 0.145;
							model = DrawCard(i, 0, rotateAngleX[i]);
						}
						else
						{
							model = DrawCard(i, 2, rotateAngleX[i]);
						}
					}
				}

				lightingShader.setMat4("model", model);
				if (i == jokerIndex)
					jCard.Draw(lightingShader);
				else
					eCard.Draw(lightingShader);
			}
			// cubes
			cubeShader.use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, cubeTexture);

			model = glm::mat4(1.0f);
			model = glm::translate(model, room);
			
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));

			unsigned int modelLoc = glGetUniformLocation(cubeShader.ID, "model");
			unsigned int viewLoc = glGetUniformLocation(cubeShader.ID, "view");
			cubeShader.setMat4("projection", projection);
			cubeShader.setMat4("view", view);
			cubeShader.setMat4("model", model);

			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			
			model = glm::mat4(1.0f);
			model = glm::translate(model, player);
			model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
			model = glm::rotate(model, glm::radians(pAngle), glm::vec3(0, 1, 0));
			cubeShader.setMat4("model", model);
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			string debug1 = string_format("Front : %f, %f, %f | Position : %f, %f, %f | Yaw : %f | Pitch : %f", camera.Front[0], camera.Front[1], camera.Front[2],
				camera.Position[0], camera.Position[1], camera.Position[2], camera.Yaw, camera.Pitch);
			string debug2 = string_format("Model Position | %f, %f, %f | Angle %f", player[0], player[1], player[2], pAngle);
			string hud = string_format("Life : %d", heart);

			RenderText(textShader, debug1, 25.0f, 25.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
			RenderText(textShader, debug2, 25.0f, 50.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
			RenderText(textShader, to_string(currentFrame), 25.0f, 75.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
			RenderText(textShader, hud, 25.0f, 650.0f, 1.25f, glm::vec3(1.0f, 0.0f, 0.0f));

			
			break;
		}
		case State::End:
		{
			break;
		}
		default:
		{
			break;
		}
		}
		

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
