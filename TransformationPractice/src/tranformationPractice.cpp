#include <stdio.h>
#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../shaders/shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../shaders/stb_image.h"



//Globals
const unsigned int SCRN_HEIGHT = 600;
const unsigned int SCRN_WIDTH = 800;

//const char* pwd = fs::current_path().file;

//Shaders
const char* vertexShaderSource = "~/OpenGLTutorial/OpenGLTutorial/shaders/vertexShader.vs";
const char* fragmentShaderSource = "~/OpenGLTutorial/OpenGLTutorial/shaders/fragmentShader.fs";

float angle = 0.0f;
float translate = 0.0f;

//Objects
float texCoords[] =
{
	0.0f, 0.0f, //lower-left corner
	1.0f, 0.0f, //lower-right corner
	0.5f, 1.0f  //top-center corner
};

float vertices[] = 
{
	// positions          // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
};

unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
};

//Prototypes
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

	/*
	//Shaders
	//Vertex Shaders
	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);

	//Fragment Shaders
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  	//Program
	unsigned int shaderProgram = createShader(vertexShader, fragmentShader);
	*/

	//Shader
	Shader ourShader(vertexShaderSource, fragmentShaderSource);


	//Vertex arrays
	unsigned int VBO, VAO;
	unsigned int EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bind vertex array object first, then bind and set vertex buffers, then configure vertex attributes
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Tex
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Texture
	unsigned int texture1;

	glGenTextures(1, &texture1);

	//glActivateTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	//Setting the texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); //Flips the image along the y-axis

	unsigned char* data = stbi_load("~/OpenGLTutorial/OpenGLTutorial/images/container.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(data);

	unsigned int texture2;
	
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	//Setting the texture wrapping/filtering options (current)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("~/OpenGLTutorial/OpenGLTutorial/images/awesomeface.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		//Alpha channel needed for png
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(data);


	//Allowed since attribpointer registered vbo, so we unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Do not unbind EBO while VAO is active since it IS stored in the buffer
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Wireframe mode`
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	ourShader.setInt("texture2", 1);

	//Unbind VAO now, so other VAO calls won't modify this one
	glBindVertexArray(0);

	

	

	float timeValue;
	float shiftValue;
	//float shapeShift = 0.7f;
	//ourShader.setFloat("xOffset", 0.0f);
	while (!glfwWindowShouldClose(window))
	{
		//Program
		timeValue = glfwGetTime();

		//Inputs
		processInput(window);

		shiftValue = (sin(timeValue * 3) / 4.0f + 0.6f);
		//vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//ourShader.setFloat("xOffset", shiftValue * shapeShift);
		//ourShader.setFloat("zoom", zoom);

		//Bind textures on corresponding units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);


		glm::mat4 trans = glm::mat4(1.0f);
		
		//Scaled by 0.5 in each direction
		trans = glm::scale(trans, glm::vec3(shiftValue, shiftValue, shiftValue));
		//Rotate by matrix trans applied zoom degrees
		trans = glm::rotate(trans, angle, glm::vec3(0.0, 0.0, 1.0));
		//Translate by matrix trans
		trans = glm::translate(trans, glm::vec3(translate, shiftValue - 0.6f, 0.0));
		//Renders
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);



		ourShader.use();

		
		//Apply tansform to the shader
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans)); //Convert the matrix so that opengl / glm match
		

		//lUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(VAO); //Must be rebound if we are changing buffers
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Deallocate
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
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
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		angle += 0.001;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		angle -= 0.001;
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		translate += 0.001;
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		translate -= 0.001;
}

