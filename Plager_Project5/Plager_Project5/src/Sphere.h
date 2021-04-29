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

class Sphere{

private:
	vec3 pos;
    float radius;
    vec3 color;
    
	// material
	float ambient;
    float diffuse;
	float phong;
    
public:
    Sphere();
    Sphere(vec3 pos, float radius, vec3 color, float ambient, float diffuse, float phong);
    ~Sphere(){};
    
    void Draw(); 
};