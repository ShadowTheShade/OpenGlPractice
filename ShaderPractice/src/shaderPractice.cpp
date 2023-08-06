#include <stdio.h>
#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../shaders/shader.h"


//Globals
const unsigned int SCRN_HEIGHT = 600;
const unsigned int SCRN_WIDTH = 800;

//Shaders
const char* vertexShaderSource = "/home/Anthony/Coding/cpp/OpenGlPractice/ShaderPractice/shaders/vertexShader.vs";
const char* fragmentShaderSource = "/home/Anthony/Coding/cpp/OpenGlPractice/ShaderPractice/shaders/fragmentShader.fs";


//Objects
float vertices[] = {
	// positions // colors
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
	0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
};

//Prototypes
static unsigned int compileShader(GLuint type, const std::string& source);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	//Initiate glfw and params
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Init window object
	GLFWwindow* window = glfwCreateWindow(SCRN_WIDTH, SCRN_HEIGHT, "LearnShaders", NULL, NULL);
	if (window == NULL) //Failed to create object
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //Change context to current window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}
	glViewport(0, 0, SCRN_WIDTH, SCRN_HEIGHT);

	//Shader
	Shader ourShader(vertexShaderSource, fragmentShaderSource);

	//Vertex arrays
	unsigned int VBO, VAO;
	//unsigned int EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	//Bind vertex array object first, then bind and set vertex buffers, then configure vertex attributes
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Col
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Allowed since attribpointer registered vbo, so we unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Do not unbind EBO while VAO is active since it IS stored in the buffer
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Unbind VAO now, so other VAO calls won't modify this one
	glBindVertexArray(0);

	//Wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



	float timeValue;
	float shiftValue;
	float triangleShift = 0.7f;
	ourShader.setFloat("xOffset", 0.0f);
	while (!glfwWindowShouldClose(window))
	{
		//Program
		timeValue = glfwGetTime();
		
		//Inputs
		processInput(window);

		//Renders
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shiftValue = (sin(timeValue) / 2.0f);
		//vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		ourShader.setFloat("xOffset", shiftValue * triangleShift);

		ourShader.use();
		//lUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(VAO); //Must be rebound if we are changing buffers
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Deallocate
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	//glDeleteProgram(shaderProgram);

	//Terminate
	glfwTerminate();
	return 0;
}

//Function passed to glfw to adjust size of the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}



