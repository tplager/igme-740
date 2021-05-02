#pragma once
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

#include "ShaderProgram.h"
#include "ShaderClass.h"

using namespace std;
using namespace glm;

class RecPrism 
{
    private:
        unsigned int num;

        uint vert_num;
        vec3* vertices;

        vec3 minPos;
        vec3 maxPos;
        vec3 color;

        // rotation matrices
        mat4 rotMat;
        mat4 invRotMat;

        // material
        float ambient;
        float diffuse;
        float phong;

        GLuint vao, vbo, nbo, ibo;

        // note that the vert and frag shaders cannot be combined with a compute shader in the same shader programs.
        // that' why we create two shader programs = vert and frag shaders are for rendering, and comp shader is for computing only
        ShaderProgram cShaderProg; // shader program for the computer shader 
        ShaderProgram vfShaderProg;  // shader program for the vertex and fragment shaders

        ShaderClass cShader; // computer shader for general-purpose computing
        ShaderClass vShader; // vertex shader
        ShaderClass fShader; // fragment shader

    public:
        RecPrism();
        ~RecPrism();

        void init(vec3 minPos, vec3 maxPos, vec3 color, mat4 rotMat, mat4 invRotMat, float ambient, float diffuse, float phong);
        void create(const char* v_shader_file, const char* f_shader_file, const char* c_shader_file); 

        void draw(mat4 viewMat, mat4 projMat, vec3 lightPos, vec3 eyePos);
};
