#pragma once
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"

class Geometry
{
private:
	std::vector<glm::vec3> vet_normals;
	glm::mat4 modelview;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> indices2;
	std::vector<GLuint> indices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;
	
public:


	glm::vec3 preset_color;
	glm::vec3 offset;
	glm::vec3 light_dir;
	glm::mat4 toWorld;
	GLuint VAO, VBO, EBO, color, NBO;
	GLuint uProjection, uModelview;
	Geometry(const char *filepath, glm::vec3 color, glm::vec3 world);

	~Geometry();
	void calculate_normals();
	void draw(int program);
	void parse(const char *filepath);
	void setup();
	void update();
	void translate(glm::vec3 move);
	void rotate();
};

