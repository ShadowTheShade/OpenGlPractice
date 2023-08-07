#define _CRT_SECURE_NO_WARNINGS

//Libraries
#include <stdio.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//math / vector / matrix libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shaders
#include "../shaders/shader.h"

//Image library
#define STB_IMAGE_IMPLEMENTATION
#include "../shaders/stb_image.h"

//Shapes / World Objects
#include "../headers/camera.h"
#include "../headers/shapes.h"

//Global

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
const unsigned int SCRN_HEIGHT = 900;
const unsigned int SCRN_WIDTH =  900;

//Timing
float currentTime = 0.0;
float deltaTime = 0.0;
float lastFrame = 0.0;

//Objects

//Shaders
const char* vertexLightingShaderSource = "/home/Anthony/Coding/cpp/OpenGlPractice/CompleteLighting/shaders/light_cube.vs";
const char* fragmentLightingShaderSource = "/home/Anthony/Coding/cpp/OpenGlPractice/CompleteLighting/shaders/light_cube.fs";

const char* vertexCubeShaderSource = "/home/Anthony/Coding/cpp/OpenGlPractice/CompleteLighting/shaders/colors.vs";
const char* fragmentCubeShaderSource = "/home/Anthony/Coding/cpp/OpenGlPractice/CompleteLighting/shaders/colors.fs";

float fov;
float lastX, lastY;

glm::vec3 cameraPos, cameraFront, cameraUp;

bool firstMouse;

// lighting
glm::vec3 lightPos(2.0f, 2.0f, 2.0f);

//World
glm::mat4 model;
glm::mat4 view;
glm::mat4 perspective;

//Square Face
//    2  1
//    3  4

float vertices[] =
{
	//Positions             //Colours               //Texture    //Normals
	0.5f,   0.5f,   0.0f,   0.0f,   0.0f,   0.0f,   1.0f, 1.0f,  //1
   -0.5f,   0.5f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f, 1.0f,  //2
   -0.5f,  -0.5f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f, 0.0f,  //3
	0.5f,  -0.5f,   0.0f,   0.0f,   0.0f,   0.0f,   1.0f, 0.0f,  //4
};

glm::vec4 normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

unsigned int indices[] =
{
	0, 1, 2,
	0, 2, 3
};

//Prototypes
static void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);
static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
static void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn);
unsigned int loadTexture(char const* path);

int main()
{
	//Initiate glfw and params
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Init window object
	GLFWwindow* window = glfwCreateWindow(SCRN_WIDTH, SCRN_HEIGHT, "SUMMARY", NULL, NULL);
	if (window == NULL) //Failed to create object
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //Set the current context to this window
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//Modes
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}
	glViewport(0, 0, SCRN_WIDTH, SCRN_HEIGHT); //Set the viewport position and size
	
	//Shaders
	Shader lightShader(vertexLightingShaderSource, fragmentLightingShaderSource);
	Shader cubeShader(vertexCubeShaderSource, fragmentCubeShaderSource);

	//Vertex arrays (vertex buffer objects / vertex array objects / element buffer objects)
	unsigned int VBO, cubeVAO, EBO;
	
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bind vertex array object and begin modifying the context
	glBindVertexArray(cubeVAO);

	//Vertex Buffer Object 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Element Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Normals
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Colours
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	
	//Textures
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Light source
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Normals
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Texture Setup
	//unsigned int texture1;
	//glGenTextures(1, &texture1);
	//glBindTexture(GL_TEXTURE_2D, texture1);
	
	//Texture 1 wrappings
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Texture 1 filtering
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Loading Image
	unsigned int diffuseMap = loadTexture("/home/Anthony/Coding/cpp/OpenGlPractice/CompleteLighting/images/container2.png");
	unsigned int specularMap = loadTexture("/home/Anthony/Coding/cpp/OpenGlPractice/CompleteLighting/images/container2_specular.png");

	//Initialize shaders
	lightShader.use();
	
	cubeShader.use();
	cubeShader.setInt("material.diffuse", 0);
	cubeShader.setInt("material.specular", 1);

	//Send texture data to shaders
	//glUniform1i(glGetUniformLocation(cubeShader.ID, "texture1"), 0);
	//cubeShader.setInt("texture1", 0);

	

	//Unbind VAO so nothing can modify it
	glBindVertexArray(0);

	//Model
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(1.0, 0.5, 0.0));
	lightShader.setMat4("model", model);
	cubeShader.setMat4("model", model);

    //View
	glm::mat4 view = glm::mat4(1.0f);
	lightShader.setMat4("view", view);
	cubeShader.setMat4("view", view);

	//Projection
	glEnable(GL_DEPTH_TEST); //Must be enabled for perspective

	glm::mat4 projection = glm::mat4(1.0f);
	lightShader.setMat4("projection", projection);
	cubeShader.setMat4("projection", projection);

	//Environment
	glm::vec3 lightColor = glm::vec3(1.0f);



	//directional lights
	const int numDirectionalLights = 1;
	glm::vec3 directionalLightDirections[numDirectionalLights] =
	{
		glm::vec3(-0.2f, -1.0f, -0.3f)
	};

	glm::vec3 directionalLightVectors[numDirectionalLights * 3] =
	{
		//ambient                       diffuse                         specular
		lightColor * glm::vec3(0.06f),  lightColor * glm::vec3(0.04f),  lightColor * glm::vec3(0.05f)
	};

	//point lights
	const int numPointLights = 1;
	glm::vec3 pointLightPositions[numPointLights] =
	{
		lightPos
	};

	glm::vec3 pointLightVectors[numPointLights * 3] =
	{
		//ambient                       diffuse                         specular
		lightColor * glm::vec3(0.06f),  lightColor * glm::vec3(0.8f),  lightColor * glm::vec3(1.0f)
	};

	float pointLightScalars[numPointLights * 3] =
	{
		//const  linear  quadratic
		1.0f,    0.09f,  0.032f
	};


	//spot lights
	const int numSpotLights = 1;
	glm::vec3 spotLightPositions[numSpotLights * 2] =
	{ //posisiton         direction
		camera.Position,  camera.Front
	};

	float spotLightScalars[numSpotLights * 3] =
	{ //cutOff                           outerCutOff
		glm::cos(glm::radians(12.5f)),   glm::cos(glm::radians(17.5f))
	};


	while (!glfwWindowShouldClose(window))
	{
		//Program
		currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		//Inputs
		processInput(window);

		//Textures

		//Transformations

		//Renders
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubeShader.use();


		//directional light | distant (usually strong) light source

		for (int i = 0; i < numDirectionalLights; i++)
		{
			char buffer[100];

			sprintf(buffer, "directionalLights[%i].ambient", i);
			cubeShader.setVec3(buffer, directionalLightVectors[i]);

			sprintf(buffer, "directionalLights[%i].diffuse", i);
			cubeShader.setVec3(buffer, directionalLightVectors[i + 1]);

			sprintf(buffer, "directionalLights[%i].specular", i);
			cubeShader.setVec3(buffer, directionalLightVectors[i + 2]);

			sprintf(buffer, "directionalLights[%i].direction", i);
			cubeShader.setVec3(buffer, directionalLightDirections[i]);
		}
		
		//point light : local (normal) light source

		for (int i = 0; i < numPointLights; i++)
		{
			char buffer[100];

			sprintf(buffer, "pointLights[%i].ambient", i);
			cubeShader.setVec3(buffer, pointLightVectors[i]);

			sprintf(buffer, "pointLights[%i].diffuse", i);
			cubeShader.setVec3(buffer, pointLightVectors[i + 1]);

			sprintf(buffer, "pointLights[%i].specular", i);
			cubeShader.setVec3(buffer, pointLightVectors[i + 2]);

			sprintf(buffer, "pointLights[%i].position", i);
			cubeShader.setVec3(buffer, pointLightPositions[i]);

			sprintf(buffer, "pointLights[%i].constant", i);
			cubeShader.setFloat(buffer, pointLightScalars[i]);

			sprintf(buffer, "pointLights[%i].linear", i);
			cubeShader.setFloat(buffer, pointLightScalars[i + 1]);

			sprintf(buffer, "pointLights[%i].quadratic", i);
			cubeShader.setFloat(buffer, pointLightScalars[i + 2]);
		}

		//spot light : local (usually strong) light source
		/*
		for (int i = 0; i < numPointLights; i++)
		{
			char buffer[100];

			sprintf(buffer, "pointLights[%i].ambient", i);
			cubeShader.setVec3(buffer, pointLightVectors[i]);

			sprintf(buffer, "pointLights[%i].diffuse", i);
			cubeShader.setVec3(buffer, pointLightVectors[i + 1]);

			sprintf(buffer, "pointLights[%i].specular", i);
			cubeShader.setVec3(buffer, pointLightVectors[i + 2]);

			sprintf(buffer, "pointLights[%i].position", i);
			cubeShader.setVec3(buffer, pointLightPositions[i]);

			sprintf(buffer, "pointLights[%i].constant", i);
			cubeShader.setFloat(buffer, pointLightScalars[i]);

			sprintf(buffer, "pointLights[%i].linear", i);
			cubeShader.setFloat(buffer, pointLightScalars[i + 1]);

			sprintf(buffer, "pointLights[%i].quadratic", i);
			cubeShader.setFloat(buffer, pointLightScalars[i + 2]);
		}
		*/


		cubeShader.setVec3("viewPos", camera.Position);
		//lightPos + glm::vec3(cos(currentTime), sin(currentTime), 0.0f)

		//View
		view = camera.GetViewMatrix();
		cubeShader.setMat4("view", view);
		
		//Perspective
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCRN_WIDTH / (float)SCRN_HEIGHT, 0.1f, 1000.0f);
		cubeShader.setMat4("projection", projection);


		//Binding textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		//Rebind VAO
		glBindVertexArray(cubeVAO);

		//Draw
		//for(int i = 0;i < 6;i++)
		//Shapes::makeCube(mainShader, glm::vec3(cos(glm::radians(60.0 * i)), sin(glm::radians(60.0 * i)), (float)i), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f));

		cubeShader.setVec3("material.ambient", 0.0215f, 0.1745f, 0.0215f);
		cubeShader.setVec3("material.diffuse", 0.07568f, 0.61424f, 0.07568f);
		cubeShader.setVec3("material.specular", 0.633f, 0.727811f, 0.633f);
		cubeShader.setFloat("material.shininess", 6.0f);
		
		for(unsigned int i = 0; i<10 ; i++)
			Shapes::makeCube(cubeShader, glm::vec3(1.0f, 2 * sin(i*20), 5 * cos(i*50)) * (i * cos(i * 20.0f)), glm::vec3(1.0f, 0.5f, 0.25f), glm::vec3(1.0f), 20.0f * i);

		
		lightShader.use();
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		lightShader.setMat4("model", model);

		glBindVertexArray(lightVAO);

		Shapes::makeCube(lightShader, lightPos, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.3f), 0.0f);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Deallocate
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	//Terminate
	glfwTerminate();
	return 0;

}

static void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.processKeyboard(DOWN, deltaTime);
	
}

static void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	//
	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos; //y coordinates go from bottom to top
	lastX = xPos;
	lastY = yPos;

	camera.processMouseMovement(xOffset, yOffset);
}


static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.processMouseScroll(static_cast<float>(yOffset));
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
