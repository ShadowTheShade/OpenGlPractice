#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>



//Globals
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

//Objects
/*float vertices[] = {
    0.5f, 0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     -0.5f,  -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};*/

float vertices[2][9] = 
{
    {
    -0.5f, 0.0f, 0.0f,
     0.0f, 0.0f, 0.0f,
    -0.25f, -1.0f, 0.0f
},
{
     0.0f, 0.0f, 0.0f,
     0.5f, 0.0f, 0.0f,
     0.25f,  1.0f, 0.0f,
    }
};


unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

//Shaders
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";

const char* fragmentShaderSource0 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


const char* fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(0.7f, 0.05f, 0.7f, 1.0f);\n"
"}\n\0";

//Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main()
{
    

    //Must init before we can use most functions
    glfwInit();
    //What options we would like to configure
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Initiate window object
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);

    //Could not allocate space : Display error and quite
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    //If the window object is made, we can change the context
    glfwMakeContextCurrent(window); //MUST SET CONTEXT BEFORE INITIALIZING GLAD
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Check if we can access glad before we call any opengl functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    glViewport(0, 0, 800, 600); //Set the viewport

    //Creating shaders
    unsigned int vertexShader;
    unsigned int fragmentShader0;
    unsigned int fragmentShader1;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader0 = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader0, 1, &fragmentShaderSource0, NULL);
    glCompileShader(fragmentShader0);

    glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShader1);

    //Shader program
    unsigned int shaderProgram0, shaderProgram1;
    shaderProgram0 = glCreateProgram();
    shaderProgram1 = glCreateProgram();

    //Add shaders to the shader program and link them
    glAttachShader(shaderProgram0, vertexShader);
    glAttachShader(shaderProgram0, fragmentShader0);
    glLinkProgram(shaderProgram0);

    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);

    //Shaders have been linked already, so no longer needed and we can free them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader0);
    glDeleteShader(fragmentShader1);


    //Buffer generation and binding
    unsigned int VBO0;
    unsigned int VBO1;
    unsigned int VAO0;
    unsigned int VAO1;
    //unsigned int EBO;
    glGenBuffers(1, &VBO0);
    //lGenBuffers(1, &VBO1);
    glGenVertexArrays(1, &VAO0);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]), vertices[0], GL_STATIC_DRAW);

    
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glGenBuffers(1, &VBO1);
    glGenVertexArrays(1, &VAO1);

    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]), vertices[1], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    //Keep window open
    while (!glfwWindowShouldClose(window))
    {
        //Input
        processInput(window);

        //Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram0); //Activate shader program
        glBindVertexArray(VAO0);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram1); //Activate shader program
        glBindVertexArray(VAO1);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //Check and call events / swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //
    glDeleteVertexArrays(1, &VAO0);
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO0);
    glDeleteBuffers(1, &VBO1);
    //glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram0);
    glDeleteProgram(shaderProgram1);

    //Cleanup
    glfwTerminate();

    return 0;
}

//Function passed to glfw to adjust the size oof the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
