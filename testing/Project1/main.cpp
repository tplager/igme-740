#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
using namespace std;

float canvasSize[] = { 25.0f, 25.0f };
int rasterSize[] = { 600, 600 };

float g_curTime = 0.0f;
float g_preTime = 0.0f;

float g_rotateSpeedA = 120.0f; // 120 degree per second
float g_rotateSpeedB = 220.0f;
float g_rotateSpeedBA = 80.0f;

float g_thetaA = 0.0f;
float g_thetaB = 0.0f;
float g_thetaBA = 0.0f;

float g_distance = 5.0f;

void init(void)
{

}


// draw a circle with radius = 1 at (0, 0)
void drawUnitCircle()
{
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 100; i++)
    {
        glColor3f(1.0f / 100.0f * (i / 10) * 10.0f + 0.2f, 0.0f, 0.0f);
        glVertex2f(cosf(3.14f * 2 / 100 * i), sinf(3.14f * 2 / 100 * i));
    }
    glEnd();
}


void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // transform and draw your stuff here ... 
    glTranslatef(5.0, 5.0f, 0.0f); 
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    //glScale(2.0f, 1.0f, 1.0f); 

    // draw the quad
    glColor3fv(color); 
    glBegin(GL_QUADS); 
    glVertex2fv(v0); 
    glVertex2fv(v1); 
    glVertex2fv(v2); 
    glVertex2fv(v3);
    glEnd(); 

    // draw the center of the quad
    glColor3f(0.0f, 0.0f, 1.0f); 
    glPointSize(10.0f); 
    glBegin(GL_POINTS); 
    glVertex2fv(quadCenter); 
    glEnd(); 
    glPointSize(1.0f); 

    // draw xy axis of the coordinate system
    drawAxis(); 

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    rasterSize[0] = w;
    rasterSize[1] = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-canvasSize[0] / 2.0f, canvasSize[0] / 2.0f, -canvasSize[0] / 2.0f, canvasSize[1] / 2.0f);
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

    // update values every frame 
    // compute the roated angles for thetaA, thetaB, and thetaBA
    g_thetaA += g_rotateSpeedA * deltaTime;
    g_thetaB += g_rotateSpeedB * deltaTime;
    g_thetaBA += g_rotateSpeedBA * deltaTime;

    g_preTime = g_curTime;

    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize((int)rasterSize[0], (int)rasterSize[1]);
    glutCreateWindow("Planet");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;

}