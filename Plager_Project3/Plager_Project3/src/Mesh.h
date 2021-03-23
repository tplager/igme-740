#ifndef MESH_H
#define MESH_H

#define GLM_FORCE_RADIANS

#include "ShaderProgram.h"

#include <vector>
#include <set>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
using namespace glm;

class Mesh
{
public:		
	/**mesh data: loaded from the file as arrays**/
	uint vert_num, tri_num;

	vec3*  vertices;
	uvec3* triangles;
	vec3* fnormals;  // normals of triangles, size = # of triangles
	vec3* vnormals;  // normal of vertices, computed by averaging the adajecent traingle normals; size = # of vertices

	GLuint vao, vbo, nbo, ibo;
	ShaderProgram shaderProg;
	ShaderClass vShader;
	ShaderClass fShader;

	mat4 modelMat;

	float normal_offset = 0.0f;

	vec3 objPos = vec3(0.0f, 2.0f, 0.0f);
	vec3 objScale = vec3(0.5f, 0.5f, 0.5f);

	GLenum fillMode = GL_FILL; 

public:
    Mesh();
    ~Mesh();

    /* Load the mesh from an '.obj' file. All faces need to be triangles.
	   Attributes like edge lengths and curvatures should be computed when simplifying the mesh.
    */
    void create (const char* filename, const  char* v_shader_file, const char* f_shader_file);

    void draw(mat4 viewMat, mat4 projMat, vec3 light1Pos, vec3 light2Pos, float time);

	vec3 getPos(); 

	void setPos(vec3 newPos); 
	void setScale(double newScale); 
	void setFillMode(GLenum newMode); 

private:
	void computeNormals(); // compute both face and vertex normals
	void prepareVBOandShaders(const char* v_shader_file, const char* f_shader_file);
};

#endif
