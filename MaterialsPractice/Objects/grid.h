#ifndef GRID_H
#define GRID_H

#include <stdlib.h>
#include <cstdlib>

//Shaders
#include "../shaders/shader.h"

//GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//math / vector / matrix libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void drawGrid(Shader shader);

class Grid {
private:
	int width;
	int height;
	int* positions;
public:
	Grid(int width, int height)
	{
		this->width = width;
		this->height = height;
		this->positions = new int[width * height];
		for (int i = 0; i < width * height; i++)
		{
			this->positions[i] = 0;
		}
		this->positions[1] = 1; //Center of grid where snake begins
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				printf("%d", positions[i+j]);
			printf("\n");
		}
	}

	void drawGrid(Shader shader)
	{
		for (int i = 0; i < this->width; i++)
		{
			for (int j = 0; j < this->height; j++)
			{
				shader.setVec3("objectColor", 0.129f, 0.129f, 0.129f);
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(-this->width / 2.0f + 0.5f, this->height / 2.0f + 0.5f, -30.0f));
				model = glm::translate(model, glm::vec3(i * 1.0f, j * -1.0f, 0.0f));

				//Cross product of faces to calculate norm
				glm::vec3 first = model * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f) - model * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f); // model * (4 - 3)
				glm::vec3 second = model * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f) - model * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f); // model * (1 - 4)
				glm::vec3 normal = glm::cross(first, second);

				shader.setVec3("normal", normal);
				shader.setMat4("model", model);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

				//
				shader.setVec3("objectColor", 0.0f, 0.0f, 0.0f);
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(-this->width / 2.0f + 0.5f, this->height / 2.0f + 0.5f, -29.9f));
				model = glm::translate(model, glm::vec3(i * 1.0f, j * -1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.9f));

				//Cross product of faces to calculate norm
				first = model * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f) - model * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f); // model * (4 - 3)
				second = model * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f) - model * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f); // model * (1 - 4)
				normal = glm::cross(first, second);

				shader.setVec3("normal", normal);
				shader.setMat4("model", model);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}
	}

	int getHeight()
	{
		return this->height;
	}

	int getWidth()
	{
		return this->width;
	}

};

#endif