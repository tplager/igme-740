
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <vector>
#include "Mesh.h"
using namespace std;

float canvasSize[] = { 10.0f, 10.0f };
int rasterSize[] = { 800, 600 };

vector<Mesh*> shapes;
Mesh* currShape; 

float color[3];

float mousePos[2];

//int maxVertices = 1; 

enum PointSize {
    Small = 11,
    Medium = 12,
    Large = 13
};

enum LineWidth {
    Thin = 14, 
    Average = 15, 
    Thick = 16
};

void init(void)
{
    mousePos[0] = mousePos[1] = 0.0f;
    color[0] = color[1] = color[2] = 0.0f;

    currShape = new Mesh(); 
    shapes.push_back(currShape); 
}

void drawCursor()
{
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex2fv(mousePos);
    glEnd();
    glPointSize(1.0f);
}

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3fv(color);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (int i = 0; i < shapes.size(); i++) {
        shapes[i]->draw(mousePos); 
    }

    currShape->draw(mousePos); 
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    rasterSize[0] = w;
    rasterSize[1] = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, canvasSize[0], 0.0, canvasSize[1]);
    glViewport(0, 0, rasterSize[0], rasterSize[1]);

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mousePos[0] = (float)x / rasterSize[0] * canvasSize[0];
        mousePos[1] = (float)(rasterSize[1] - y) / rasterSize[1] * canvasSize[1];

        if (currShape->getMaxVertices() != -1) {
            currShape->addVertex(mousePos[0], mousePos[1]);

            if (currShape->getNumVertices() >= currShape->getMaxVertices()) {
                currShape = new Mesh(currShape->getMaxVertices(), currShape->getColor(), currShape->getPointSize(), currShape->getLineWidth());

                shapes.push_back(currShape);
            }

            glutPostRedisplay();
        }
    }
}

void motion(int x, int y)
{
    // mouse events are handled by OS, eventually. When using mouse in the raster window, it assumes top-left is the origin.
    // Note: the raster window created by GLUT assumes bottom-left is the origin.
    mousePos[0] = (float)x / rasterSize[0] * canvasSize[0];
    mousePos[1] = (float)(rasterSize[1] - y) / rasterSize[1] * canvasSize[1];

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 13: 
            if (currShape->getMaxVertices() == INT_MAX) {
                currShape = new Mesh(currShape->getMaxVertices(), currShape->getColor(), currShape->getPointSize(), currShape->getLineWidth());
            }

            glutPostRedisplay();
            break;
        case 27:
            exit(0);
            break;
    }
}

void menu(int value)
{
    switch (value) {
    case 0: // clear
        currShape = new Mesh(currShape->getMaxVertices(), currShape->getColor(), currShape->getPointSize(), currShape->getLineWidth());

        shapes.clear(); 
        shapes.push_back(currShape);

        glutPostRedisplay();
        break;
    case 1: //exit
        exit(0);
        break;
    case 2:                         // Set object to Point
        if (currShape->getNumVertices() == 0) {
            currShape->setMaxVertices(1);
        }            
        break;       
    case 3:                         // Set object to Line
        if (currShape->getNumVertices() == 0) {
            currShape->setMaxVertices(2);
        }            
        break;       
    case 4:                          // Set object to Triangle
        if (currShape->getNumVertices() == 0) {
            currShape->setMaxVertices(3);
        }            
        break;       
    case 5:                          // Set object to Quad
        if (currShape->getNumVertices() == 0) {
            currShape->setMaxVertices(4);
        }            
        break;       
    case 6:                          // Set object to Polygon
        if (currShape->getNumVertices() == 0) {
            currShape->setMaxVertices(INT_MAX);
        }
        break;
    case 7:                         // Set color to Red
        currShape->setColor(1.0f, 0.0f, 0.0f);
        glutPostRedisplay();
        break;
    case 8:                         // Set color to Green
        currShape->setColor(0.0f, 1.0f, 0.0f);
        glutPostRedisplay();
        break;
    case 9:                         // Set color to Blue
        currShape->setColor(0.0f, 0.0f, 1.0f);
        glutPostRedisplay();
        break;
    case 10:                        // Set color to Black
        currShape->setColor(0.0f, 0.0f, 0.0f);
        glutPostRedisplay(); 
        break; 
    case 11:                        // Set point size to Small
        currShape->setPointSize(1.0f);
        break;                              
    case 12:                        // Set point size to Medium
        currShape->setPointSize(5.0f);
        break;
    case 13:                        // Set point size to Large
        currShape->setPointSize(10.0f);
        break;
    case 14:                        // Set line width to Thin
        currShape->setLineWidth(1.0f);
        break;
    case 15:                        // Set line width to Average
        currShape->setLineWidth(2.5f);
        break;
    case 16:                        // Set line width to Thick
        currShape->setLineWidth(5.0f);
        break;
    default:
        break;
    }
}
void createMenu()
{
    int objectMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Point", 2);
    glutAddMenuEntry("Line", 3);
    glutAddMenuEntry("Triangle", 4);
    glutAddMenuEntry("Quad", 5);
    glutAddMenuEntry("Polygon", 6);

    int colorMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Red", 7);
    glutAddMenuEntry("Green", 8);
    glutAddMenuEntry("Blue", 9);
    glutAddMenuEntry("Black", 10); 

    int sizesMenu = glutCreateMenu(menu); 
    glutAddMenuEntry("Small", PointSize::Small); 
    glutAddMenuEntry("Medium", PointSize::Medium); 
    glutAddMenuEntry("Large", PointSize::Large); 

    int widthsMenu = glutCreateMenu(menu); 
    glutAddMenuEntry("Thin", LineWidth::Thin); 
    glutAddMenuEntry("Average", LineWidth::Average); 
    glutAddMenuEntry("Thick", LineWidth::Thick); 

    glutCreateMenu(menu);
    glutAddMenuEntry("Clear", 0);
    glutAddSubMenu("Objects", objectMenu); 
    glutAddSubMenu("Colors", colorMenu);
    glutAddSubMenu("Point Size", sizesMenu); 
    glutAddSubMenu("LineWidth", widthsMenu);
    glutAddMenuEntry("Exit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char* argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(rasterSize[0], rasterSize[1]);
    glutCreateWindow("2D Drawing");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    createMenu();
    glutMainLoop();

    return 0;
}