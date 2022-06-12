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
	Model clown(FileSystem::getPath("Data/clown/Standing Melee Attack Downward.dae"));
	Model character(FileSystem::getPath("Data/character/Running.dae"));
	
	Animation hammerAnimation(FileSystem::getPath("Data/clown/Standing Melee Attack Downward.dae"), &clown);
	Animation characterAnimation(FileSystem::getPath("Data/character/Running.dae"), &character);

	Animator hammerAnimator(&hammerAnimation);
	Animator characterAnimator(&characterAnimation);

	Shader textShader("shader/text.vs", "shader/text.fs");
	Shader tableShader("shader/table.vs", "shader/table.fs");
	Shader jCardShader("shader/jokercard.vs", "shader/jokercard.fs");
	Shader eCardShader("shader/emptycard.vs", "shader/emptycard.fs");
	Shader cubeShader("shader/cube.vs", "shader/cube.fs");
	Shader lightingShader("shader/multiple_lights.vs", "shader/multiple_lights.fs");
	Shader lobbyShader("shader/lobby.vs", "shader/lobby.fs");
	Shader animShader("shader/anim_model.vs", "shader/anim_model.fs");
	Shader characterShader("shader/anim_model.vs", "shader/anim_model.fs");
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);


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
	hammerAnimator.UpdateAnimation(deltaTime);
	characterAnimator.UpdateAnimation(deltaTime);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		//std::cout << "deltatime : " << deltaTime << endl;
		currentSpeed = (deltaTime / AVG_DELTATIME) * baseSpeed;
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
			string help3 = string_format("3. Move Character with Keyboard Arrows");
			string help4 = string_format("4. Find Joker in 3 times");
			string help5 = string_format("p.s. Hope you good luck...");

			RenderText(textShader, help1, 150.0f, 500.0f, 1.25f, glm::vec3(1.0, 0.0f, 0.0f));
			RenderText(textShader, help2, 150.0f, 425.0f, 1.25f, glm::vec3(1.0, 0.0f, 0.0f));
			RenderText(textShader, help3, 150.0f, 350.0f, 1.25f, glm::vec3(1.0, 0.0f, 0.0f));
			RenderText(textShader, help4, 150.0f, 275.0f, 1.25f, glm::vec3(1.0, 0.0f, 0.0f));
			RenderText(textShader, help5, 150.0f, 150.0f, 1.0f, glm::vec3(1.0, 0.0f, 0.0f));

			break;
		}
		case State::InGame:
		{
			if (jokerAnimation == AnimationState::Hammering)
			{
				hammerAnimator.UpdateAnimation(deltaTime);
				jokerAnimationLastTime += deltaTime;
				if (hammerAnimator.GetCurrentTime() < jokerAnimationLastTime)
				{
					jokerAnimationLastTime = 0.0f;
					jokerAnimation = AnimationState::AniIdle;
					sequence = GameSequence::Check;
					sequenceStartTime = static_cast<float>(glfwGetTime());
				}
				else {
					jokerAnimationLastTime = hammerAnimator.GetCurrentTime();

					if (jokerAnimationLastTime / hammerAnimation.GetDuration() > 0.5f)
					{
						selected[min(selectCard + 1, 6)] = (selected[min(selectCard + 1, 6)] == 2) ? 2 : 1;
						selected[max(selectCard -1 , 0)] = (selected[max(selectCard - 1, 0)] == 2) ? 2 : 1;
						selected[selectCard] = (selected[selectCard] == 2) ? 2 : 1;
						if (selectCard == jokerIndex)
						{
							jokerCardFound = TRUE;
						}
					}
				}
			}

			if (playerAnimation == AnimationState::Running)
			{
				characterAnimator.UpdateAnimation(deltaTime);
				
			}
			
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			animShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 100.0f);
			glm::mat4  view = camera.GetViewMatrix();


			animShader.setMat4("projection", projection);
			animShader.setMat4("view", view);
			// material properties
			auto transforms = hammerAnimator.GetFinalBoneMatrices();
			for (int i = 0; i < transforms.size(); ++i)
				animShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, jokerModel);
			model = glm::rotate(model, glm::radians(jokerFacingAngle), glm::vec3(0.0f, 1.0f, 0.0f));
			 // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(.3f, .3f, .3f));
			animShader.setMat4("model", model);

			clown.Draw(animShader);

			///further development

		//	auto BoneRight = hammerAnimation.FindBone("RightHand");
		//	auto localtransform = hammerAnimator.GetFinalBoneMatrices()[23];

		//	lightingShader.use();
		//	//model *= localtransform;
		//	model = glm::mat4(1.0f);
		//	/*model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));*/
		//	model = hammerAnimation.FindBone("RightHand")->GetLocalTransform() *;
		///*	model = glm::scale(model, glm::vec3(.3f, .3f, .3f));
		//	model = hammerAnimator.GetFinalBoneMatrices()[23];*/
		//	// translate it down so it's at the center of the scene
		//	
		//
		//	/*model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));*/
		//	
		//	lightingShader.setMat4("projection", projection);
		//	lightingShader.setMat4("view", view);
		//	lightingShader.setMat4("model", model);
		//	hammer.Draw(lightingShader);


			characterShader.use();
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 100.0f);
			view = camera.GetViewMatrix();


			characterShader.setMat4("projection", projection);
			characterShader.setMat4("view", view);
			// material properties
			auto charactertransforms = characterAnimator.GetFinalBoneMatrices();
			for (int i = 0; i < charactertransforms.size(); ++i)
				characterShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", charactertransforms[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, player);
			// translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(.05f, .05f, .05f));
			model = glm::rotate(model, glm::radians(pAngle+180.0f), glm::vec3(0, 1, 0));
			characterShader.setMat4("model", model);

			character.Draw(characterShader);
			hammer.Draw(characterShader);
			lightingShader.use();
			lightingShader.setVec3("viewPos", camera.Position);
			lightingShader.setFloat("material.shininess", 32.0f);
			// directional light
			lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
			lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
			lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
			lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

			// point light 1
			lightingShader.setVec3("pointLights[0].position", glm::vec3(0, 3, 0));
			lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
			lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
			lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("pointLights[0].constant", 1.0f);
			lightingShader.setFloat("pointLights[0].linear", 0.09f);
			lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

			// spotLight
			lightingShader.setVec3("spotLight.position", glm::vec3(card_mid[selectCard].x, 0.5, card_mid[selectCard].z));
			lightingShader.setVec3("spotLight.direction", glm::vec3(0, -1, 0));
			lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
			lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("spotLight.constant", 1.0f);
			lightingShader.setFloat("spotLight.linear", 0.09f);
			lightingShader.setFloat("spotLight.quadratic", 0.032f);
			lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

			// material properties
			lightingShader.setFloat("material.shininess", 32.0f);

			// view/projection transformations
			
			lightingShader.setMat4("projection", projection);
			lightingShader.setMat4("view", view);

			
			// view/projection transformations
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 100.0f);
			 view = camera.GetViewMatrix();
			lightingShader.setMat4("projection", projection);
			lightingShader.setMat4("view", view);

			// world transformation
			 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
			lightingShader.setMat4("model", model);
			table.Draw(lightingShader);


			for (int i = 0; i < 7; i++)
			{
				if (selected[i] == 0)
				{
					card[i].y = 0.145;
					isCardGround[i] = true;
					model = DrawCard(i, 0, rotateAngleX[i]);	// it's a bit too big for our scene, so scale it down
				}
				else if (selected[i] == 2)
				{
					rotateAngleX[i] = -90.f;
					rotateAngleY[i] = 180.f;
					rotateAngleZ[i] = -cAngle[i];
					card[i].y = 0.195;
					isCardGround[i] = true;
					model = DrawCard(i, 5, rotateAngleY[i]);
				}
				else if (selected[i] == 1)
				{
					if (i == selectCard)
					{
						if (rotateAngleY[i] < 90.f)
						{
							isCardGround[i] = false;
							model = DrawCard(i, 3, rotateAngleY[i]);
						}
						else if (rotateAngleY[i] > 180.f)
						{
							selected[i] = 2;
							rotateAngleX[i] = -90.f;
							rotateAngleY[i] = 180.f;
							rotateAngleZ[i] = -cAngle[i];
							card[i].y = 0.195;
							isCardGround[i] = true;
							model = DrawCard(i, 5, rotateAngleY[i]);
						}
						else
						{
							isCardGround[i] = false;
							model = DrawCard(i, 4, rotateAngleY[i]);
						}
					}
					else
					{
						if (rotateAngleX[i] < 90.f)
						{
							isCardGround[i] = false;
							model = DrawCard(i, 1, rotateAngleX[i]);
						}
						else if (rotateAngleX[i] > 270.f)
						{
							selected[i] = 0;
							rotateAngleX[i] = -90.f;
							card[i].y = 0.145;
							isCardGround[i] = true;
							model = DrawCard(i, 0, rotateAngleX[i]);
						}
						else
						{
							isCardGround[i] = false;
							model = DrawCard(i, 2, rotateAngleX[i]);
						}
					}
				}

				isGround = true;
				for (int i = 0; i < 7; i++)
				{
					if (!isCardGround[i])
						isGround = false;
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

			while (particle.size() < MAX_PARTICLE)
			{
				auto ticle =  particles();
				rand() % 2 > 1 ? ticle.active = true : ticle.active = false;
				ticle.pos = player + glm::vec3(rand() % 10*0.1, rand() %10*0.1 , rand() %10*0.1);
				ticle.life = rand() % 10 ;
				ticle.vel = glm::normalize( glm::vec3(rand() % 10*0.01, rand() % 10*0.01, rand() % 10*0.01));
				ticle.rotate = rand() % 360;
				ticle.roataeSpeed = rand() % 4;
				particle.push_back(ticle);
			}

			for (auto& ticle : particle)
			{
				ticle.life -= 0.1f;
				if (ticle.life < 0.0f)
				{
					if (playerAnimation == AnimationState::Running)
					{
						ticle.life = rand() % 10;
						ticle.pos = player;
					}
					continue;
				}

				model = glm::mat4(1.0f);
				model = glm::translate(model, ticle.pos + ticle.vel * baseSpeed * 0.1f);
				model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
				model = glm::rotate(model, glm::radians(ticle.rotate ), glm::vec3(0, 1, 0));
				cubeShader.setMat4("model", model);
				glBindVertexArray(cubeVAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindVertexArray(0);
				ticle.pos += ticle.vel * baseSpeed*0.1f ;
				ticle.active ? ticle.rotate += ticle.roataeSpeed : ticle.rotate -= ticle.roataeSpeed;
			}


			

			string debug1 = string_format("Front : %f, %f, %f | Position : %f, %f, %f | Yaw : %f | Pitch : %f", camera.Front[0], camera.Front[1], camera.Front[2],
				camera.Position[0], camera.Position[1], camera.Position[2], camera.Yaw, camera.Pitch);
			string debug2 = string_format("Model Position | %f, %f, %f | Angle %f | IsGround : %d", player[0], player[1], player[2], pAngle, isGround);
			string hud = string_format("Life : %d", heart);

			RenderText(textShader, debug1, 25.0f, 25.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
			RenderText(textShader, debug2, 25.0f, 50.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
			RenderText(textShader, to_string(currentFrame), 25.0f, 75.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f));
			RenderText(textShader, hud, 25.0f, 650.0f, 1.25f, glm::vec3(1.0f, 0.0f, 0.0f));

			if (sequence == GameSequence::Ready)
			{
				if (sequenceStartTime == 0.0f)
				{
					sequenceStartTime = static_cast<float>(glfwGetTime());;
					
				}
				if( currentFrame - sequenceStartTime <= 1 )
					RenderText(textShader, "Ready", SCR_WIDTH/2-150.0f, SCR_HEIGHT/2-75.0f, 3.0f, glm::vec3(0.5, 0.8f, 0.2f));
				else if (currentFrame - sequenceStartTime <= 2 )
					RenderText(textShader, "Start!", SCR_WIDTH / 2 - 150.0f, SCR_HEIGHT / 2 - 75.0f, 3.0f, glm::vec3(0.5, 0.8f, 0.2f));
				else {
					sequence = GameSequence::CountDown;
					sequenceStartTime = static_cast<float>(glfwGetTime());;
				}

			}

			if (sequence == GameSequence::CountDown)
			{
				if (currentFrame - sequenceStartTime  <= 1)
					RenderText(textShader, "3", SCR_WIDTH / 2 - .01f *SCR_WIDTH, SCR_HEIGHT / 2 + 0.14f * SCR_WIDTH, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
				else if (currentFrame - sequenceStartTime  <= 2)
					RenderText(textShader, "2", SCR_WIDTH / 2 - .01f * SCR_WIDTH, SCR_HEIGHT / 2 + 0.14f * SCR_WIDTH, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
				else if (currentFrame - sequenceStartTime  <= 3)
					RenderText(textShader, "1!", SCR_WIDTH / 2 - .01f * SCR_WIDTH, SCR_HEIGHT / 2 + 0.14f * SCR_WIDTH, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
				else {
					sequence = GameSequence::Hammer;
					jokerAnimation = AnimationState::Hammering;
					sequenceStartTime = static_cast<float>(glfwGetTime());
				}

			}

			if (sequence == GameSequence::Check)
			{
				if (isGround)
				{
					if (jokerCardFound)
					{
						sequence = GameSequence::Finish;						
					}
					else {
						heart -=1;
						if (heart > 0)
						{
							sequence = GameSequence::CountDown;
						}
						else {
							sequence = GameSequence::Finish;
						}
					}
				}
				sequenceStartTime = static_cast<float>(glfwGetTime());
			}

			if (sequence == GameSequence::Finish)
			{
				if (currentFrame - sequenceStartTime <= 3) {

					if (jokerCardFound)
					{
						RenderText(textShader, "Victory!", SCR_WIDTH / 2 - 150.0f, SCR_HEIGHT / 2 - 75.0f, 3.0f, glm::vec3(0.5, 0.8f, 0.2f));
					}
					else {
						RenderText(textShader, "Lose!", SCR_WIDTH / 2 - 150.0f, SCR_HEIGHT / 2 - 75.0f, 3.0f, glm::vec3(0.5, 0.8f, 0.2f));
					}
				}
				else {
					sequence = GameSequence::Ready;
					gameState = State::Lobby;
					jokerCardFound = false;
					heart = 3;
					jokerIndex = rand() % 7;
					jokerCardFound = false;
					for (int i = 0; i < 7; i++)
					{
						selected[i] = 0;
						rotateAngleX[i] = -90.f;
						rotateAngleY[i] = 0.f;
						rotateAngleZ[i] = cAngle[i];
					}
				}
			}

			playerAnimation = AnimationState::AniIdle;
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


	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
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
