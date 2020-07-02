#pragma once
#ifndef OBJLOADER_H
#define OBJLOADER_H


bool loadOBJ(const char* filename, std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals, std::vector< GLfloat > &k, std::vector< GLfloat > & y, std::vector< GLfloat > & z);


bool loadAssImp(
	const char * path,
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
);

#endif
class objloader
{
public:
	objloader();
	~objloader();
};

