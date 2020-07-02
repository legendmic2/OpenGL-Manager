#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "./include/GL/glew.h"
#include "glm/gtc/matrix_transform.hpp"
class my_obj
{
public:
	my_obj();
	~my_obj();
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;;
	GLuint axas;
	bool res;
	float objcolor[3];
	float obj_opacity;
	int colornum;

};

