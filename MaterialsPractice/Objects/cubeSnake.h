#ifndef SNAKE_H
#define SNAKE_H

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

#include "grid.h"

extern Grid grid;
extern GLFWwindow* window;

enum Snake_Movement {
	sUP,
	sDOWN,
	sLEFT,
	sRIGHT,
	sStop
};

glm::vec3 distances[] =
{
	glm::vec3(0.0f, 0.05f, 0.0f),  //UP
	glm::vec3(0.0f, -0.05f, 0.0f), //DOWN
	glm::vec3(-0.05f, 0.0f, 0.0f), //LEFT
	glm::vec3(0.05f, 0.0f, 0.0f)   //RIGHT
};

//
class CubeNode {
private:
	CubeNode* next;
	CubeNode* prev;
	int gridX;
	int gridY;
	glm::vec3 location;
	glm::vec3 color;
	Snake_Movement currentDir;
	Snake_Movement prevDir;
public:
	CubeNode(int gridX, int gridY)
	{
		this->next = NULL;
		this->prev = NULL;
		this->color = glm::vec3((rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f);
		this->currentDir = sStop;
		this->prevDir = sStop;
		this->gridX = gridX;
		this->gridY = gridY;
		this->location = glm::vec3(gridX - 15.0f, 11.0f - gridY, -30.0f);
	}

	glm::vec3 getLocation()
	{
		return this->location;
	}

	glm::vec3 getColor()
	{
		return this->color;
	}

	void translateNode(Snake_Movement dir)
	{
		switch (dir)
		{
		case sUP:
			this->location += glm::vec3(0.0f, 0.05f, 0.0f);
			break;
		case sDOWN:
			this->location += glm::vec3(0.0f, -0.05f, 0.0f);
			break;
		case sLEFT:
			this->location += glm::vec3(-0.05f, 0.0f, 0.0f);
			break;
		case sRIGHT:
			this->location += glm::vec3(0.05f, 0.0f, 0.0f);
			break;
		default:
			break;
		}

	}

	CubeNode* getNext()
	{
		return this->next;
	}

	CubeNode* getPrev()
	{
		return this->prev;
	}

	void setNext(CubeNode* node)
	{
		this->next = node;
	}

	void setPrev(CubeNode* node)
	{
		this->prev = node;
	}

	Snake_Movement getCurrentDir()
	{
		return this->currentDir;
	}

	Snake_Movement getPrevDir()
	{
		return this->prevDir;
	}

	void setCurrentDir(Snake_Movement dir)
	{
		switch (dir)
		{
		case sUP:
			this->gridY -= 1;
			break;
		case sDOWN:
			this->gridY += 1;
			break;
		case sLEFT:
			this->gridX -= 1;
			break;
		case sRIGHT:
			this->gridX += 1;
			break;
		default:
			break;
		}
		this->currentDir = dir;
	}

	void setPrevDir(Snake_Movement dir)
	{
		this->prevDir = dir;
	}

	int getX()
	{
		return this->gridX;
	}

	int getY()
	{
		return this->gridY;
	}
	
};

class CubeSnake {
private:
	CubeNode* head;
	CubeNode* tail;
	Snake_Movement nextDir;
	int size;
public:
	CubeSnake()
	{
		//No snake is without a head
		this->head = new CubeNode(15, 10);
		this->tail = this->head;
		this->head->setCurrentDir(sUP);
		this->head->setNext(this->head);
		this->head->setPrev(this->head);
		this->nextDir = sStop;
		this->size = 1;
	}

	void changeDir(Snake_Movement dir)
	{
		this->nextDir = dir;
		CubeNode* travel = this->head;
		travel->setPrevDir(travel->getCurrentDir());
		travel->setCurrentDir(this->nextDir);
		checkCollision();
		travel = travel->getNext();
		while (travel != this->head)
		{
			travel->setPrevDir(travel->getCurrentDir());
			travel->setCurrentDir(travel->getPrev()->getPrevDir());
			travel = travel->getNext();
		}
	}

	void translateSnake()
	{
		CubeNode* travel = this->head;
		travel->translateNode(travel->getCurrentDir());
		travel = travel->getNext();
		while (travel != this->head)
		{
			travel->translateNode(travel->getCurrentDir());
			travel = travel->getNext();
		}
	}

	CubeNode* getHead()
	{
		return this->head;
	}

	void addCube(CubeNode* node)
	{
		this->tail->setNext(node);
		node->setPrev(this->tail);
		node->setNext(this->head);
		this->tail = node;
		this->size++;
	}

	void drawSnake(Shader shader)
	{
		CubeNode* tracker = this->head;
		shader.setVec3("objectColor", tracker->getColor());
		Shapes::makeCube(shader, tracker->getLocation(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.9f), 0.0f);
		tracker = tracker->getNext();
		while (tracker != this->head)
		{
			shader.setVec3("objectColor", tracker->getColor());
			Shapes::makeCube(shader, tracker->getLocation(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.9f), 0.0f);
			tracker = tracker->getNext();
		}
	}

	void checkCollision()
	{
		CubeNode* head = this->head;
		if (head->getX() < 0 || head->getX() >= grid.getWidth() || head->getY() < 0 || head->getY() >= grid.getHeight())
			glfwSetWindowShouldClose(window, true);
	}

	void deleteSnake()
	{
		CubeNode* prev = this->head;
		CubeNode* travel = this->head;
		travel->getPrev()->setNext(NULL);
		travel = travel->getNext();
		while (travel != NULL)
		{
			free(prev);
			prev = travel;
			travel = travel->getNext();
		}
		free(prev);
	}
	
};



#endif