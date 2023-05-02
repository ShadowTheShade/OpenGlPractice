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


//Globals

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
const unsigned int SCRN_HEIGHT = 900;
const unsigned int SCRN_WIDTH =  900;

//Timing
double currentTime = 0.0;
double deltaTime = 0.0;
double lastFrame = 0.0;

//Objects
const char* vertexShaderSource = "D:/Coding/OpenGL/OpenGlTutorial/shaders/vertexShader.vs";
const char* fragmentShaderSource = "D:/Coding/OpenGL/OpenGlTutorial/shaders/fragmentShader.fs";


float fov;
float lastX, lastY;

glm::vec3 cameraPos, cameraFront, cameraUp;

bool firstMouse;


//Multiple Cubes
//    2  1
//    3  4
float vertices[] =
{
	//Positions             //Colours               //Texture
	0.5f,   0.5f,   0.0f,   0.0f,   0.0f,   0.0f,   1.0f, 1.0f, //1
   -0.5f,   0.5f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f, 1.0f, //2
   -0.5f,  -0.5f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f, 0.0f, //3
	0.5f,  -0.5f,   0.0f,   0.0f,   0.0f,   0.0f,   1.0f, 0.0f, //4
};

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
	Shader mainShader(vertexShaderSource, fragmentShaderSource);


	//Vertex arrays (vertex buffer objects / vertex array objects / element buffer objects)
	unsigned int VBO, VAO, EBO;
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bind vertex array object and begin modifying the context
	glBindVertexArray(VAO);

	//Vertex Buffer Object 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Element Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Colours
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	//Textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Light source
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//Texture Setup
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	//Texture 1 wrappings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Texture 1 filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Loading Image
	int texWidth, texHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("D:/Coding/OpenGl/OpenGlTutorial/images/Shadow.jpg", &texWidth, &texHeight, &nrChannels, 0);
	
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(data);

	//Initialize shader
	mainShader.use();

	//Send texture data to shaders
	glUniform1i(glGetUniformLocation(mainShader.ID, "texture1"), 0);
	//mainShader.setInt("texture1", 0);

	//Unbind VAO so nothing can modify it
	glBindVertexArray(0);

	//Model
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(1.0, 0.5, 0.0));
	mainShader.setMat4("model", model);

    //View
	glm::mat4 view = glm::mat4(1.0f);
	mainShader.setMat4("view", view);

	//Projection
	glEnable(GL_DEPTH_TEST); //Must be enabled for perspective

	glm::mat4 projection = glm::mat4(1.0f);
	mainShader.setMat4("projection", projection);

	while (!glfwWindowShouldClose(window))
	{
		//Program
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		//Inputs
		processInput(window);

		//Textures

		//Transformations

		//Renders
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mainShader.use();


		//View
		view = camera.GetViewMatrix();
		mainShader.setMat4("view", view);
		
		//Perspective
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCRN_WIDTH / (float)SCRN_HEIGHT, 0.1f, 1000.0f);
		mainShader.setMat4("projection", projection);


		//Binding textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		//Rebind VAO
		glBindVertexArray(VAO);

		//Draw
		//for(int i = 0;i < 6;i++)
			//Shapes::makeCube(mainShader, glm::vec3(cos(glm::radians(60.0 * i)), sin(glm::radians(60.0 * i)), (float)i), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f));
		
		Shapes::makeCube(mainShader, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Deallocate
	glDeleteVertexArrays(1, &VAO);
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
