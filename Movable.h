#pragma once
#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

class Movable
{
public:
	// pure virtual function providing interface framework.
	virtual ~Movable() {}
	virtual void move() = 0;
	void setDestination(glm::vec3 d) {
		destination = d;
	}

protected:
	float speed = 1.0;
	bool isMoving;
	glm::vec3 destination;
};

