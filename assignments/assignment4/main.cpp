#include <stdio.h>
#include <math.h>

#include "ew/external/stb_image.h"
#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <woah/shader.h>
#include <woah/texture.h>

void processInput(GLFWwindow* window);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const float CUBE_SPEED = 15.0f;

int  success;
char infoLog[512];

// cube variables
// local (vertex) position
float vertices[] = {
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

// world position
glm::vec3 cubePositions[20];
glm::vec3 cubeRotations[20];
glm::vec3 cubeScales[20];

// camera variables
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// time variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
	//Initialization goes here!
	// 
	// cube position/scale/rotation generation
	srand(time(NULL));
	for (int i = 0; i < 20; i++) {
		cubePositions[i] = glm::vec3(
			ew::RandomRange(-10.0f, 10.0f),
			ew::RandomRange(-5.0f, 5.0f),
			ew::RandomRange(-20.0f, -5.0f)
		);

		cubeRotations[i] = glm::vec3(
			ew::RandomRange(0.0f, 360.0f),
			ew::RandomRange(0.0f, 360.0f),
			ew::RandomRange(0.0f, 360.0f)
		);

		float scale = ew::RandomRange(0.75f, 2.25f);
		cubeScales[i] = glm::vec3(scale, scale, scale);
	}

	glEnable(GL_DEPTH_TEST);
	Shader shaderProgram("assets/shader.vert", "assets/shader.frag");
    glEnable(GL_BLEND);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    //textures
	Texture cubeFace("assets/gramma_bg.png", GL_NEAREST, GL_REPEAT);


	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//Clear framebuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Drawing happens here!
		cubeFace.Bind(0);

		shaderProgram.use();
		shaderProgram.setInt("texture1", 0);

		// camera
		glm::mat4 view = glm::mat4(1.0f);
		float radius = 10.0f;
		float camX = static_cast<float>(sin(glfwGetTime()) * radius);
		float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shaderProgram.setMat4("view", view);

		// create transformations
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

		unsigned int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		
		shaderProgram.setMat4("projection", projection);

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 20; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			// position
			model = glm::translate(model, cubePositions[i]);

			// rotation
			cubeRotations[i][0] += deltaTime * cubeRotations[i].x / CUBE_SPEED;
			cubeRotations[i][1] += deltaTime * cubeRotations[i].y / CUBE_SPEED;
			cubeRotations[i][2] += deltaTime * cubeRotations[i].z / CUBE_SPEED;

			model = glm::rotate(model, glm::radians(cubeRotations[i][0]), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(cubeRotations[i][1]), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(cubeRotations[i][2]), glm::vec3(0.0f, 0.0f, 1.0f));


			// scale
			model = glm::scale(model, cubeScales[i]);

			shaderProgram.setMat4("model", model);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float BaseCameraSpeed = static_cast<float>(2.5 * deltaTime);
	float cameraSpeed = BaseCameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed = BaseCameraSpeed * 2.0f;
	else
		cameraSpeed = BaseCameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;
}