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

static void makeCube(Shader shader, glm::vec3 translate = glm::vec3(0.0f), glm::vec3 rotate = glm::vec3(1.0f), glm::vec3 scale = glm::vec3(1.0f));

class Shapes {
public:
	static void makeCube(Shader shader, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale)
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
			//Translations                 //Rotations
			glm::vec3(-0.5f, 0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f), //1
			glm::vec3(0.0f, -0.5f,  0.0f), glm::vec3(1.0f, 0.0f,  0.0f), //2
			glm::vec3(0.5f,  0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f), //3
			glm::vec3(0.0f,  0.5f,  0.0f), glm::vec3(1.0f, 0.0f,  0.0f), //4
			glm::vec3(0.0f,  0.0f, -0.5f), glm::vec3(0.0f, 0.0f,  1.0f), //5
			glm::vec3(0.0f,  0.0f,  0.5f), glm::vec3(0.0f, 0.0f,  1.0f), //6
		};

		//Draw
		//Model
		glm::mat4 model;
		for (int i = 0; i < 6; i++)
		{
			model = glm::mat4(1.0f);
			//Entire object movement
			model = glm::translate(model, translate);
			model = glm::rotate(model, (float)glfwGetTime(), rotate);

			//Personal face movement
			model = glm::translate(model, cubeSides[i * 2]);
			model = glm::rotate(model, glm::radians(90.0f), cubeSides[(i * 2) + 1]);

			shader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

	}

};


#endif