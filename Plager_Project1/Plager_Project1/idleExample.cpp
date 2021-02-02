
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
using namespace std;

float canvasSize[] = { 10.0f, 10.0f };
int rasterSize[] = { 800, 600 };

// define the center and offset of a quad
float quadCenter[2] = { 5.0f, 2.5f };
float quadOffset = 1.0f;


float g_curTime = 0.0f;
float g_preTime = 0.0f;

float g_moveDis = 0.0f;
float g_moveSpeed = 0.5f;

void init(void)
{

}

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(g_moveDis, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glVertex2f(quadCenter[0] - quadOffset, quadCenter[1] + quadOffset);
    glVertex2f(quadCenter[0] + quadOffset, quadCenter[1] + quadOffset);
    glVertex2f(quadCenter[0] + quadOffset, quadCenter[1] - quadOffset);
    glVertex2f(quadCenter[0] - quadOffset, quadCenter[1] - quadOffset);
    glEnd();

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

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;
    }
}

void idle()
{
    g_curTime = glutGet(GLUT_ELAPSED_TIME); // in millisecond
    float deltaTime = (float)(g_curTime - g_preTime) / 1000.0f; // delta time is converted to seconds 

    g_moveDis += g_moveSpeed * deltaTime;

    g_preTime = g_curTime;

    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize((int)rasterSize[0], (int)rasterSize[1]);
    glutCreateWindow("idle function - move a quad");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;

}