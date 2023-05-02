#ifndef SHAPES_H
#define SHAPES_H

//Shaders
#include "../shaders/shader.h"

//GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//math / vector / matrix libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"

extern glm::mat4 model;



static void makeCube(Shader shader, glm::vec3 translate = glm::vec3(0.0f), glm::vec3 rotate = glm::vec3(1.0f), glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f);

class Shapes {
public:
	static void makeCube(Shader shader, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale, float angle)
	{

		//               4
		//           __________      5(back) 
		//         /|         /|
		//        /__________/ |
		//       |  |       |  |
		// 1     |  |_______|_ |   3
		//       | /        | /
		//       |/         |/
		//        ----------
		//6(front)     2  

		glm::vec3 cubeSides[] =
		{
			//Translations                 //Rotation 1                  
			glm::vec3(-0.5f, 0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f),  //1
			glm::vec3(0.0f, -0.5f,  0.0f), glm::vec3(1.0f, 0.0f,  0.0f),  //2
			glm::vec3(0.5f,  0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f),  //3
			glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(-1.0f, 0.0f,  0.0f),  //4
			glm::vec3(0.0f,  0.0f, -0.5f), glm::vec3(1.0f, 0.0f,  0.0f),  //5
			glm::vec3(0.0f,  0.0f,  0.5f), glm::vec3(1.0f, 0.0f,  0.0f),  //6
		};

		float cubeAngles[] =
		{   //1    //2
			90.0f,  //1
			90.0f,  //2
			90.0f,  //3
			90.0f, //4
			180.0f, //5
			0.0f,    //6
		};

		//Draw
		//Model
		
		for (int i = 0; i < 6; i++)
		{
			
			model = glm::mat4(1.0f);
			//Entire object movement
			model = glm::translate(model, translate);
			model = glm::rotate(model, glm::radians(angle), rotate);
			model = glm::scale(model, scale);
			//Personal face movement
			model = glm::translate(model, cubeSides[i * 2]);
			model = glm::rotate(model, glm::radians(cubeAngles[i]), cubeSides[(i * 2) + 1]); //Rotation 1
			//model = glm::rotate(model, glm::radians(cubeAngles[i*2 +1]), cubeSides[(i * 3) + 2]); //Rotation 2

			/*
			//Multiple Cubes
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
			*/
			//Cross product of faces to calculate norm
			glm::vec3 first = model * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f) - model * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f); // model * (4 - 3)
			glm::vec3 second = model * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f) - model * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f); // model * (1 - 4)
			glm::vec3 normal = glm::cross(first, second);

			//std::cout << i << std::endl;
			//std::cout << "FIRST: " << glm::to_string(first) << std::endl;
			//std::cout << "SECOND: " << glm::to_string(normal) << std::endl;
			//std::cout << "NORMAL: " << glm::to_string(normal) << std::endl;

			shader.setVec3("normal", normal);
			shader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

	}

};


#endif