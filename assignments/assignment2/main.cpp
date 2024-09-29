#include <stdio.h>
#include <math.h>

#include "ew/external/stb_image.h"
#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <woah/shader.h>
#include <woah/texture.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

int  success;
char infoLog[512];

float vertices[] = {
    // positions          // colors           // texture coords
    // Background
    -1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // top left
     1.0f,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // top right
     1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,  // bottom left 

    // Character
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left 
};

unsigned int indices[] = {
    // Background
    0, 1, 2, // first triangle
    2, 3, 0, // second triangle

    // Character
    4, 5, 6, // first triangle
    6, 7, 4  // second triangle
};

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
	Shader bgShader("assets/bgShader.vert", "assets/bgShader.frag");
    Shader charShader("assets/charShader.vert", "assets/charShader.frag");
    glEnable(GL_BLEND);

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

    //textures
    Texture character("assets/gramma.png", GL_NEAREST, GL_CLAMP_TO_EDGE);
    Texture bg("assets/gramma_bg.png", GL_NEAREST, GL_REPEAT);

	
	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float time = (float)glfwGetTime();

		//Clear framebuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Drawing happens here!
		//bg
		bgShader.use();
		int timeLoc = glGetUniformLocation(bgShader.ID, "uTime");
		glUniform1f(timeLoc, time);
		bg.Bind(0);
		glUniform1i(glGetUniformLocation(bgShader.ID, "texture1"), 0);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//character
        charShader.use();
		timeLoc = glGetUniformLocation(charShader.ID, "uTime");
		glUniform1f(timeLoc, time * 3);
		character.Bind(1);
		glUniform1i(glGetUniformLocation(charShader.ID, "texture2"), 1);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
