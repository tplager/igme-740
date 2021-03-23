// This OpenGL project demonstrates a simple interactive camera implementation and a few vertex and fragment shaders. 
// Two modes of camera controls (press c to switch between two modes): 
// 1. Focus Mode: Holding ALT and Left Mouse Button (LMB) and moving the mouse will rotate the camera about the LookAt Position
//                Holding ALT and MMB and moving the mouse will pan the camera.
//                Holding ALT and RMB and moving the mouse will zoom the camera.
// 2. First-Person Mode: Pressing W, A, S, or D will move the camera
//                       Holding LMB and moving the mouse will roate the camera.
// Basic shader - demonstrate the basic use of vertex and fragment shader
// Displacement shader - a special fireball visual effects with Perlin noise function
// Toon shading shader - catoonish rendering effects
// Per-vertex shading v.s. per-fragment shading = visual comparison between two types of shading 

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "PhysicalLight.h"

#include <iostream>
using namespace std;
using namespace glm;

int g_winWidth  = 1024;
int g_winHeight = 512;

Camera g_cam;

unsigned char g_keyStates[256];

struct shaderFileGroup {
	const char* v_shader_file;
	const char* f_shader_file;
};

shaderFileGroup teapotShaderFiles[2] = {
	{".\\Shaders\\perFrag_phong.vert", ".\\Shaders\\perFrag_phong.frag"},
	{".\\Shaders\\perVert_phong.vert", ".\\Shaders\\perVert_phong.frag"},
};

//char light_v_shader_file[] =
////".\\Shaders\\basic.vert";
////".\\Shaders\\displacement.vert"; // vertex displacement shader with perlin noise
//".\\Shaders\\perVert_lambert.vert"; // basic lambert lighting  
////".\\Shaders\\perFrag_lambert.vert"; // basic lambert lighting with per-fragment implementation
////".\\Shaders\\toon_shading.vert"; // basic toon shading with per-fragment implementation
//
//char light_f_shader_file[] =
////".\\Shaders\\basic.frag";
////".\\Shaders\\displacement.frag"; // vertex displacement shader with perlin noise
//".\\Shaders\\perVert_lambert.frag"; // basic lambert shading 
////".\\Shaders\\perFrag_lambert.frag"; // basic lambert shading with per-fragment implementation
////".\\Shaders\\toon_shading.frag"; // basic toon shading with per-fragment implementation

const char teapotModel[128] = "Models/teapot.obj";

const char sphereModel[128] = "Models/sphere.obj";

Mesh g_teapots[2];

PhysicalLight g_lightSpheres[2];

PhysicalLight* g_selectedLight; 
double g_lightMoveSpeed = 0.05; 

float g_time = 0.0f;

void initialization() 
{    
    g_cam.set(3.0f, 4.0f, 14.0f, 0.0f, 1.0f, -0.5f, g_winWidth, g_winHeight);

	for (int i = 0; i < 2; i++) {
		g_teapots[i].create(teapotModel, teapotShaderFiles[i].v_shader_file, teapotShaderFiles[i].f_shader_file);
	}
	g_teapots[1].setPos(vec3(3, 2, 0));

	g_lightSpheres[0].setPos(vec3(3, 3, 3));
	g_lightSpheres[1].setPos(vec3(1, 0, -2));

	g_selectedLight = &g_lightSpheres[0];
	g_lightSpheres[0].select();

	// add any stuff you want to initialize ...
}

/****** GL callbacks ******/
void initialGL()
{    
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	
	glClearColor (1.0f, 1.0f, 1.0f, 0.0f);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

void idle()
{
    // add any stuff to update at runtime ....

    g_cam.keyOperation(g_keyStates, g_winWidth, g_winHeight);

	glutPostRedisplay();
}

void display()
{	 
	glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// add any stuff you'd like to draw	

	glUseProgram(0);
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	g_cam.drawGrid();
    g_cam.drawCoordinateOnScreen(g_winWidth, g_winHeight);
    g_cam.drawCoordinate();

	g_time = (float)glutGet(GLUT_ELAPSED_TIME)/1000.0f;

	g_lightSpheres[0].draw();
	g_lightSpheres[1].draw();

	g_teapots[0].draw(g_cam.viewMat, g_cam.projMat, g_lightSpheres[0].getPos(), g_lightSpheres[1].getPos(), g_time);;
	g_teapots[1].draw(g_cam.viewMat, g_cam.projMat, g_lightSpheres[0].getPos(), g_lightSpheres[1].getPos(), g_time);;

    glutSwapBuffers();
}

void reshape(int w, int h)
{
	g_winWidth = w;
	g_winHeight = h;
	if (h == 0) {
		h = 1;
	}
	g_cam.setProjectionMatrix(g_winWidth, g_winHeight);
    g_cam.setViewMatrix();
    glViewport(0, 0, w, h);
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

// Need to add controls for 
void keyboard(unsigned char key, int x, int y)
{
    g_keyStates[key] = true;
	switch(key) { 
		case 27:
			exit(0);
			break;
        case ' ':
            g_cam.PrintProperty();
            break;
		case 's': 
			g_selectedLight->moveLight(vec3(0, 0, g_lightMoveSpeed)); 
			break; 
		case 'w':
			g_selectedLight->moveLight(vec3(0, 0, -g_lightMoveSpeed));
			break; 
		case 'd':
			g_selectedLight->moveLight(vec3(g_lightMoveSpeed, 0, 0));
			break; 
		case 'a':
			g_selectedLight->moveLight(vec3(-g_lightMoveSpeed, 0, 0));
			break;
		case 'u':
			g_selectedLight->moveLight(vec3(0, g_lightMoveSpeed, 0));
			break;
		case 'j':
			g_selectedLight->moveLight(vec3(0, -g_lightMoveSpeed, 0));
			break;
		case 49: 
			if (g_selectedLight == &g_lightSpheres[1]) {
				g_selectedLight = &g_lightSpheres[0]; 
				g_lightSpheres[0].select(); 
				g_lightSpheres[1].deselect(); 
			}
			break;
		case 50: 
			if (g_selectedLight == &g_lightSpheres[0]) {
				g_selectedLight = &g_lightSpheres[1];
				g_lightSpheres[1].select();
				g_lightSpheres[0].deselect();
			}
			break;
	}
}

int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(g_winWidth, g_winHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("VertFrag Shader Example");
	
	glewInit();
	initialGL();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
    glutKeyboardUpFunc(keyup);
    glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	initialization();
	
    glutMainLoop();
    return EXIT_SUCCESS;
}