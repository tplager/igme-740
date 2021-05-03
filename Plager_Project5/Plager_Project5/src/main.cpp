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

#include "Camera.h"

#include "Sphere.h"
#include "Box.h"
#include "Light.h"

#include "Text.h"

#include "ShaderClass.h"
#include "ShaderProgram.h"

#include <fstream>
#include <iostream>

#ifndef MATH_PI 
#define MATH_PI 3.14159265f
#endif

using namespace glm;

int g_winWidth = 640;
int g_winHeight = 480;

Camera g_cam;
Text g_text;
unsigned char g_keyStates[256];

const char dataFile[128] = "geoData/geo.txt";

const GLchar c_shader_file[] = ".\\shaders\\c_shader.comp";

ShaderClass cShader; 
ShaderProgram cShaderProg; 

float vertices[4 * 2] = 
{
    0,   0,
    640, 0,
    640, 480,
    0,   480
}; 

float texCoords[4 * 2] = 
{ 
    0, 0, 
    1, 0, 
    1, 1, 
    0, 1 
};

GLuint glTexID = -1;

unsigned int g_box_num;
Box* g_boxes;

unsigned int g_sphere_num;
Sphere * g_spheres;

Light g_light;

void loadConfigFile(const char* pFilePath)
{
	const unsigned int CAMERA = 0;
	const unsigned int LIGHT = 1;
	const unsigned int SPHERE = 2;
	const unsigned int BOX = 3;

	vec3 rot;
	std::fstream filestr;
	filestr.open(pFilePath, std::fstream::in);
	if (!filestr.is_open())
	{
		cout << "'" << pFilePath << "'" << ", geo file does not exsit!" << endl;
		filestr.close();
		return;
	}

	char objType[80];
	char attrType[80];
	unsigned int objState = -1;

	bool loop = true;
	while (loop)
	{
		filestr >> objType;
		if (!strcmp(objType, "Camera:")) objState = CAMERA;
		else if (!strcmp(objType, "Light:")) objState = LIGHT;
		else if (!strcmp(objType, "Sphere:")) objState = SPHERE;
		else if (!strcmp(objType, "Box:")) objState = BOX;
		switch (objState)
		{
		case CAMERA:
			float e_x, e_y, e_z;
			float l_x, l_y, l_z;
			float near_plane;

			for (int i = 0; i < 3; i++)
			{
				filestr >> attrType;
				if (!strcmp(attrType, "eye:"))
				{
					filestr >> e_x;
					filestr >> e_y;
					filestr >> e_z;
				}
				else if (!strcmp(attrType, "lookat:"))
				{
					filestr >> l_x;
					filestr >> l_y;
					filestr >> l_z;
				}
				else if (!strcmp(attrType, "near_plane:")) filestr >> near_plane;
			}
			g_cam.set(e_x, e_y, e_z, l_x, l_y, l_z, g_winWidth, g_winHeight, 45.0f, near_plane, 1000.0f);
			break;
		case LIGHT:
			filestr >> attrType;
			if (!strcmp(attrType, "position:"))
			{
				filestr >> g_light.pos.x;
				filestr >> g_light.pos.y;
				filestr >> g_light.pos.z;
			}
			filestr >> attrType;
			if (!strcmp(attrType, "color:"))
			{
				filestr >> g_light.color.x;
				filestr >> g_light.color.y;
				filestr >> g_light.color.z;
			}
			filestr >> attrType;
			if (!strcmp(attrType, "intensity:"))
			{
				filestr >> g_light.intensity;
			}
			break;
		case SPHERE:
			filestr >> attrType;
			if (!strcmp(attrType, "num:"))
				filestr >> g_sphere_num;
			if (g_sphere_num > 0)
			{
				g_spheres = new Sphere[g_sphere_num];

				for (int i = 0; i < g_sphere_num; i++)
				{
					filestr >> attrType;
					if (!strcmp(attrType, "position:"))
					{
						filestr >> g_spheres[i].pos.x; filestr >> g_spheres[i].pos.y;  filestr >> g_spheres[i].pos.z;
					}
					filestr >> attrType;
					if (!strcmp(attrType, "radius:"))
					{
						filestr >> g_spheres[i].radius;
					}
					filestr >> attrType;
					if (!strcmp(attrType, "color:"))
					{
						filestr >> g_spheres[i].color.x; filestr >> g_spheres[i].color.y;  filestr >> g_spheres[i].color.z;
					}
					filestr >> attrType;
					if (!strcmp(attrType, "ambient:"))	filestr >> g_spheres[i].ambient;
					filestr >> attrType;
					if (!strcmp(attrType, "diffuse:"))	filestr >> g_spheres[i].diffuse;
					filestr >> attrType;
					if (!strcmp(attrType, "phong:")) 	filestr >> g_spheres[i].phong;

				}
			}
			break;
		case BOX:
			filestr >> attrType;
			if (!strcmp(attrType, "num:"))
				filestr >> g_box_num;

			if (g_box_num > 0)
			{
				g_boxes = new Box[g_box_num];
				for (int i = 0; i < g_box_num; i++)
				{
					filestr >> attrType;
					if (!strcmp(attrType, "conner_position:"))
					{
						filestr >> g_boxes[i].minPos.x;
						filestr >> g_boxes[i].minPos.y;
						filestr >> g_boxes[i].minPos.z;
						filestr >> g_boxes[i].maxPos.x;
						filestr >> g_boxes[i].maxPos.y;
						filestr >> g_boxes[i].maxPos.z;
					}

					filestr >> attrType;
					if (!strcmp(attrType, "color:"))
					{
						filestr >> g_boxes[i].color.x;
						filestr >> g_boxes[i].color.y;
						filestr >> g_boxes[i].color.z;
					}
					filestr >> attrType;
					if (!strcmp(attrType, "rotate:"))
					{
						filestr >> rot.x;
						filestr >> rot.y;
						filestr >> rot.z;
						//Convert to radians
						rot.x *= 3.14159265f / 180.0f;
						rot.y *= 3.14159265f / 180.0f;
						rot.z *= 3.14159265f / 180.0f;

						mat4 m(1.0f);


						// rotation order is zyx
						m = rotate(m, rot.z, vec3(0, 0, 1));
						m = rotate(m, rot.y, vec3(0, 1, 0));
						m = rotate(m, rot.x, vec3(1, 0, 0));

						//cout<<glm::to_string(m)<<endl;

						g_boxes[i].rotMat = m;
						g_boxes[i].invRotMat = inverse(m);
					}
					filestr >> attrType;
					if (!strcmp(attrType, "ambient:"))	filestr >> g_boxes[i].ambient;
					filestr >> attrType;
					if (!strcmp(attrType, "diffuse:"))	filestr >> g_boxes[i].diffuse;
					filestr >> attrType;
					if (!strcmp(attrType, "phong:"))	filestr >> g_boxes[i].phong;
				}
				loop = false;
			}
			break;
		}
	}
}

void setShaderUniforms() {
	// Camera
    cShaderProg.setFloat3V("eye", 1, glm::value_ptr(g_cam.eye));
	cShaderProg.setFloat("camAngle", g_cam.fovy * MATH_PI / 180.0f);
    cShaderProg.setFloat("nearPlane", g_cam.near_plane);

	// Light
    cShaderProg.setFloat3V("sceneLight.pos", 1, glm::value_ptr(g_light.pos));
    cShaderProg.setFloat3V("sceneLight.color", 1, glm::value_ptr(g_light.color));
    cShaderProg.setFloat("sceneLight.intensity", g_light.intensity);

	// Spheres
    for (int i = 0; i < g_sphere_num; i++) {
        cShaderProg.setFloat3V(("spheres[" + std::to_string(i) + "].pos").data(), 1, glm::value_ptr(g_spheres[i].pos));
        cShaderProg.setFloat(("spheres[" + std::to_string(i) + "].rad").data(), g_spheres[i].radius);
        cShaderProg.setFloat3V(("spheres[" + std::to_string(i) + "].color").data(), 1, glm::value_ptr(g_spheres[i].color));
        
		// Sphere Lighting
		cShaderProg.setFloat(("spheres[" + std::to_string(i) + "].ambient").data(), g_spheres[i].ambient);
        cShaderProg.setFloat(("spheres[" + std::to_string(i) + "].diffuse").data(), g_spheres[i].diffuse);
        cShaderProg.setFloat(("spheres[" + std::to_string(i) + "].phong").data(), g_spheres[i].phong);
    }

	// Boxes
    for (int i = 0; i < g_box_num; i++) {
        cShaderProg.setFloat3V(("boxes[" + std::to_string(i) + "].min").data(), 1, glm::value_ptr(g_boxes[i].minPos));
        cShaderProg.setFloat3V(("boxes[" + std::to_string(i) + "].max").data(), 1, glm::value_ptr(g_boxes[i].maxPos));
        cShaderProg.setMatrix4fv(("boxes[" + std::to_string(i) + "].rot").data(), 1, glm::value_ptr(g_boxes[i].rotMat));
		cShaderProg.setMatrix4fv(("boxes[" + std::to_string(i) + "].invRot").data(), 1, glm::value_ptr(g_boxes[i].invRotMat));
        cShaderProg.setFloat3V(("boxes[" + std::to_string(i) + "].color").data(), 1, glm::value_ptr(g_boxes[i].color));
        
		// Box Lighting
		cShaderProg.setFloat(("boxes[" + std::to_string(i) + "].ambient").data(), g_spheres[i].ambient);
        cShaderProg.setFloat(("boxes[" + std::to_string(i) + "].diffuse").data(), g_spheres[i].diffuse);
        cShaderProg.setFloat(("boxes[" + std::to_string(i) + "].phong").data(), g_spheres[i].phong);
    }
}
void createTexture()
{
	glGenTextures(1, &glTexID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, glTexID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// send the imagedata (on CPU) to the GPU memory at glTexID (glTexID is a GPU memory location index)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, g_winWidth, g_winHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(0, glTexID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void initialization() 
{    
    loadConfigFile(dataFile);

    // adding stuff to draw ...
    cShader.create(c_shader_file, GL_COMPUTE_SHADER);
    cShaderProg.create();
    cShaderProg.link(cShader); // link compute shader 
    cShader.destroy();

	createTexture();

	glUseProgram(cShaderProg.id);

    setShaderUniforms();

	glDispatchCompute((GLuint)g_winWidth, (GLuint)g_winHeight, 1);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

/****** GL callbacks ******/
void idle()
{
    // adding stuff to update at runtime ....

    g_cam.keyOperation(g_keyStates, g_winWidth, g_winHeight);
}

void drawPlane()
{
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, glTexID); // use the texture on the quad 
	glBegin(GL_QUADS);
	glTexCoord2fv(texCoords + 2 * 0); glVertex2fv(vertices + 2 * 0);
	glTexCoord2fv(texCoords + 2 * 1); glVertex2fv(vertices + 2 * 1);
	glTexCoord2fv(texCoords + 2 * 2); glVertex2fv(vertices + 2 * 2);
	glTexCoord2fv(texCoords + 2 * 3); glVertex2fv(vertices + 2 * 3);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawPlane();

    //glUseProgram(0);
    //// draw sphere and box
    //for (int i=0; i<g_sphere_num; i++)
    //    g_spheres[i].draw();
    //for (int i=0; i<g_box_num; i++)
    //    g_boxes[i].draw();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
	g_winWidth = w;
	g_winHeight = h;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);

	glViewport(0, 0, (GLsizei)g_winWidth, (GLsizei)g_winHeight);
}

void mouse(int button, int state, int x, int y)
{
	g_cam.mouseClick(button, state, x, y, g_winWidth, g_winHeight);
}

void motion(int x, int y)
{
	g_cam.mouseMotion(x, y, g_winWidth, g_winHeight);
}

void keyup(unsigned char key, int x, int y)
{
	g_keyStates[key] = false;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	}
}


int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize((int)g_winWidth, (int)g_winHeight);

	glutCreateWindow("Ray casting");

	glewInit();

	initialization();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return EXIT_SUCCESS;
}