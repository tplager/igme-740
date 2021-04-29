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
using namespace std;
using namespace glm;

class RecPrism 
{
    private:
        unsigned int num;

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

    public:
        RecPrism();
        RecPrism(vec3 minPos, vec3 maxPos, vec3 color, mat4 rotMat, mat4 invRotMat, float ambient, float diffuse, float phong); 
        ~RecPrism() {};

        vec3 getMinPos();
        vec3 getMaxPos();
        vec3 getColor();

        void Draw();
};
