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

	Shader textShader("shader/text.vs", "shader/text.fs");
	Shader tableShader("shader/table.vs", "shader/table.fs");
	Shader jCardShader("shader/jokercard.vs", "shader/jokercard.fs");
	Shader eCardShader("shader/emptycard.vs", "shader/emptycard.fs");
	Shader cubeShader("shader/cube.vs", "shader/cube.fs");

	// load textures
// -------------
	unsigned int cubeTexture = loadTexture(FileSystem::getPath("resources/textures/container.jpg").c_str());


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


			string debug1 = string_format(" Mouse X : %f  Mouse Y : %f", lastX, lastY);
			RenderText(textShader, debug1, 25.0f, 25.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));

			break;
		}
		case State::InGame:
		{
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

			// render the joker card
			jCardShader.use();
			jCardShader.setMat4("projection", projection);
			jCardShader.setMat4("view", view);
			model = glm::mat4(1.0f);
			model = glm::translate(model, card[jokerIndex]); // translate it down so it's at the center of the scene
			model = glm::rotate(model, glm::radians(cAngle[jokerIndex]), glm::vec3(0, 1, 0));
			model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
			model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));	// it's a bit too big for our scene, so scale it down
			jCardShader.setMat4("model", model);
			jCard.Draw(jCardShader);

			// render the empty card
			eCardShader.use();
			eCardShader.setMat4("projection", projection);
			eCardShader.setMat4("view", view);
			for (int i = 0; i < 7; i++)
			{
				if (i == jokerIndex)
					continue;

				model = glm::mat4(1.0f);
				model = glm::translate(model, card[i]); // translate it down so it's at the center of the scene
				model = glm::rotate(model, glm::radians(cAngle[i]), glm::vec3(0, 1, 0));
				model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
				model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));	// it's a bit too big for our scene, so scale it down
				eCardShader.setMat4("model", model);
				eCard.Draw(eCardShader);
			}

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
			string debug2 = string_format("Model Position | %f, %f, %f | Angle %f", player[0], player[1], player[2], pAngle);

			RenderText(textShader, debug1, 25.0f, 25.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
			RenderText(textShader, debug2, 25.0f, 50.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
			RenderText(textShader, to_string(currentFrame), 25.0f, 75.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
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

