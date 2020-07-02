#include "stdafx.h"

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "glm/glm.hpp"
#include "./include/GL/glew.h"
#include "objloader.h"

using namespace std;
// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

std::vector< glm::vec2 > textureuvs;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
std::vector<unsigned short> velements;
std::vector<unsigned short> telements;
std::vector<unsigned short> nelements;

bool loadOBJ(const char* filename, std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals, std::vector< GLfloat > &k, std::vector< GLfloat > & y, std::vector< GLfloat > & z) {
	std::vector< glm::vec2 > textureuvs;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned short> velements;
	std::vector<unsigned short> telements;
	std::vector<unsigned short> nelements;
	ifstream in(filename, ios::in);
	if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }
	vector<int> nb_seen;

	string line;
	while (getline(in, line)) {
		if (line.substr(0, 2) == "v ") {
			istringstream s(line.substr(2));
			glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;// v.w = 1.0;
			k.push_back(v.x);// End the drawing
			y.push_back(v.y);// End the drawing
			z.push_back(v.z);// End the drawing
			vertices.push_back(v);
		}
		else if (line.substr(0, 3) == "vt ") {
			istringstream s(line.substr(3));
			glm::vec2 vt; s >> vt.x; s >> vt.y;
			vt.y = -vt.y;
			textureuvs.push_back(vt);
		}
		else if (line.substr(0, 3) == "vn ") {
			istringstream s(line.substr(3));
			glm::vec3 vn; s >> vn.x; s >> vn.y; s >> vn.z;
			normals.push_back(vn);
		}
		else if (line.substr(0, 2) == "f ") {
			istringstream s(line.substr(2));
			unsigned short av, bv, cv;
			av = bv = cv = 0;
			unsigned short at, bt, ct;
			at = bt = ct = 0;
			unsigned short an, bn, cn;
			an = bn = cn = 0;
			char c, count;
			count = 0;
			char col, op, eq;
			string ss;
			char c2;
			for (int j = 0; j < 3; j++) {
				getline(s, ss, ' ');
				count = 0;
				istringstream a(ss);
				if (j == 0) {
					c2 = a.peek();
					if (isdigit(c2)) {
						a >> av;
						c2 = a.peek();
					}
					while (!isdigit(c2)) {
						count++;
						a >> col;
						c2 = a.peek();
					}
					if (count == 2)
						a >> an;
					else if (count == 1) {
						a >> at;
						c2 = a.peek();
						if (!isdigit(c2))
						{
							a >> col;
							a >> an;
						}
					}
				}
				else if (j == 1)
				{
					c2 = a.peek();
					if (isdigit(c2)) {
						a >> bv;
						c2 = a.peek();
					}
					while (!isdigit(c2)) {
						count++;
						a >> col;
						c2 = a.peek();
					}
					if (count == 2)
						a >> bn;
					else if (count == 1) {
						a >> bt;
						c2 = a.peek();
						if (!isdigit(c2))
						{
							a >> col;
							a >> bn;
						}
					}
				}
				else if (j == 2)
				{
					c2 = a.peek();
					if (isdigit(c2)) {
						a >> cv;
						c2 = a.peek();
					}
					while (!isdigit(c2)) {
						count++;
						a >> col;
						c2 = a.peek();
					}
					if (count == 2)
						a >> cn;
					else if (count == 1) {
						a >> ct;
						c2 = a.peek();
						if (!isdigit(c2))
						{
							a >> col;
							a >> cn;
						}
					}
				}
				else {
					col = ' ';
				}
			}
			if (av != 0)
				av--;
			if (at != 0)
				at--;
			if (an != 0)
				an--;
			if (bv != 0)
				bv--;
			if (bt != 0)
				bt--;
			if (bn != 0)
				bn--;
			if (cv != 0)
				cv--;
			if (ct != 0)
				ct--;
			if (cn != 0)
				cn--;
			velements.push_back(av); telements.push_back(at); nelements.push_back(an);
			velements.push_back(bv); telements.push_back(bt); nelements.push_back(bn);
			velements.push_back(cv); telements.push_back(ct); nelements.push_back(cn);
			//printf("¹®Á¦ %d \n", an);
		}
		else if (line[0] == '#') {}
		else {}

	}
	std::vector<glm::vec3> tempnormal;
	std::vector<glm::vec3> pNormals;

	if (vertices.size() > 0 && normals.empty()) {
		for (int q = 0; q < velements.size(); q += 3)
		{
			GLushort ia = velements[q];
			GLushort ib = velements[q + 1];
			GLushort ic = velements[q + 2];

			glm::vec3 tempnormala = glm::cross(
				glm::vec3(vertices[ia]) - glm::vec3(vertices[ic]),
				glm::vec3(vertices[ic]) - glm::vec3(vertices[ib]));
			tempnormal.push_back(tempnormala);
			tempnormal.push_back(tempnormala);
			tempnormal.push_back(tempnormala);
			glm::vec3 normal = glm::normalize(glm::cross(
				glm::vec3(vertices[ia]) - glm::vec3(vertices[ic]),
				glm::vec3(vertices[ic]) - glm::vec3(vertices[ib])));
			nelements[q] = ia;
			nelements[q + 1] = ib;
			nelements[q + 2] = ic;
			pNormals.push_back(normal);

			//normals[ia] = normals[ib] = normals[ic] = normal;
		}
		std::vector<glm::vec3> tempnormal2;
		glm::vec3 vSum = { 0,0,0 };
		glm::vec3 vZero = vSum;
		glm::vec3 tempnormal21;
		int shared = 0;
		for (int k = 0; k < vertices.size(); k++)			// Go through all of the vertices
		{
			for (int j = 0; j < velements.size(); j += 3)	// Go through all of the triangles
			{												// Check if the vertex is shared by another face
				if (velements[j] == k ||//pObject->pFaces[j].vertIndex[0] == i ||
					velements[j + 1] == k ||
					velements[j + 2] == k)
				{
					vSum = vSum + tempnormal[j];// Add the un-normalized normal of the shared face
					shared++;								// Increase the number of shared triangles
				}
			}

			// Get the normal by dividing the sum by the shared.  We negate the shared so it has the normals pointing out.
			tempnormal21 = vSum / float(-shared);
			// Normalize the normal for the final vertex normal
			tempnormal21 = glm::normalize(tempnormal21);
			normals.push_back(tempnormal21);
			vSum = vZero;									// Reset the sum
			shared = 0;										// Reset the shared
		}
	}
	


	/*mesh->normals.resize(mesh->vertices.size(), glm::vec3(0.0, 0.0, 0.0));
	nb_seen.resize(mesh->vertices.size(), 0);
	for (unsigned int i = 0; i < mesh->velements.size(); i += 3) {
		GLushort via = mesh->velements[i];
		GLushort vib = mesh->velements[i + 1];
		GLushort vic = mesh->velements[i + 2];
	}
	for (unsigned int i = 0; i < mesh->velements.size(); i += 3) {
		GLushort ia = mesh->velements[i];
		GLushort ib = mesh->velements[i + 1];
		GLushort ic = mesh->velements[i + 2];

		GLushort ia = mesh->velements[i];
		GLushort ib = mesh->velements[i + 1];
		GLushort ic = mesh->velements[i + 2];
		*/
		/*glm::vec3 normal = glm::normalize(glm::cross(
			glm::vec3(mesh->vertices[ib]) - glm::vec3(mesh->vertices[ia]),
			glm::vec3(mesh->vertices[ic]) - glm::vec3(mesh->vertices[ia])));
			*/
			//int v[3];  v[0] = ia;  v[1] = ib;  v[2] = ic;
			/*for (int j = 0; j < 3; j++) {
				GLushort cur_v = v[j];
				nb_seen[cur_v]++;
				if (nb_seen[cur_v] == 1) {
					mesh->normals[cur_v] = normal;
				}
				else {
					// average
					mesh->normals[cur_v].x = mesh->normals[cur_v].x * (1.0 - 1.0 / nb_seen[cur_v]) + normal.x * 1.0 / nb_seen[cur_v];
					mesh->normals[cur_v].y = mesh->normals[cur_v].y * (1.0 - 1.0 / nb_seen[cur_v]) + normal.y * 1.0 / nb_seen[cur_v];
					mesh->normals[cur_v].z = mesh->normals[cur_v].z * (1.0 - 1.0 / nb_seen[cur_v]) + normal.z * 1.0 / nb_seen[cur_v];
					mesh->normals[cur_v] = glm::normalize(mesh->normals[cur_v]);
				}
			}*/
	for (unsigned int i = 0; i < velements.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = velements[i];
		unsigned int uvIndex = telements[i];
		unsigned int normalIndex = nelements[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = vertices[vertexIndex];
		glm::vec2 uv;
		if (!textureuvs.empty())
			uv = textureuvs[uvIndex];
		glm::vec3 normal;
		if (!normals.empty())
			normal = normals[normalIndex];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		if (!textureuvs.empty())
			out_uvs.push_back(uv);
		if (!normals.empty())
			out_normals.push_back(normal);
		//printf("%d...\n", uvIndex);
	}

}



/*
bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
){
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);

	}

	return true;
}



*/
#ifdef USE_ASSIMP // don't use this #define, it's only for me (it AssImp fails to compile on your machine, at least all the other tutorials still work)

// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

bool loadAssImp(
	const char * path,
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
) {

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, 0/*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
	if (!scene) {
		fprintf(stderr, importer.GetErrorString());
		getchar();
		return false;
	}
	const aiMesh* mesh = scene->mMeshes[0]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)

	// Fill vertices positions
	vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D pos = mesh->mVertices[i];
		vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
	}

	// Fill vertices texture coordinates
	uvs.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
		uvs.push_back(glm::vec2(UVW.x, UVW.y));
	}

	// Fill vertices normals
	normals.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D n = mesh->mNormals[i];
		normals.push_back(glm::vec3(n.x, n.y, n.z));
	}


	// Fill face indices
	indices.reserve(3 * mesh->mNumFaces);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		// Assume the model has only triangles.
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}

	// The "scene" pointer will be deleted automatically by "importer"

}

#endif

objloader::objloader()
{
}


objloader::~objloader()
{
}
