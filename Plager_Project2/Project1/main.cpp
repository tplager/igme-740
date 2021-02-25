//Usage:
//Hold down the number keys , 1-7, to select one or multiple circles.
//While circle(s) are selected, use the left mouse button to translate and use the right mouse button to rotate.

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <math.h>
#include "Robot.h"

using namespace std;

int win_width = 600, win_height = 600;
float canvas_width = 20.0f; float canvas_height = 20.0f;

bool keyStates[256];
int buttonState;

Robot robot; 

void init(void)
{
    for (int i = 0; i < 256; i++) {
        keyStates[i] = false;
    }

    buttonState = -1;
}

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    robot.draw(); 

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    win_width = w;
    win_height = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-canvas_width / 2.0f, canvas_width / 2.0f, -canvas_height / 2.0f, canvas_height / 2.0f);
    glViewport(0, 0, (GLsizei)win_width, (GLsizei)win_height);

    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
    if (key == 27) // 'esc' key
        exit(0);
    else if (key == 'a') {
        robot.rotateCounterClockwise(); 
    }
    else if (key == 'd') {
        robot.rotateClockwise(); 
    }

    glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) 
{
    switch (key)
    {
        case GLUT_KEY_UP:
            robot.selectUp();
            break;
        case GLUT_KEY_DOWN:
            robot.selectDown();
            break;
        case GLUT_KEY_LEFT:
            robot.selectLeft();
            break;
        case GLUT_KEY_RIGHT:
            robot.selectRight();
            break;
    }

    glutPostRedisplay(); 
}

int main(int argc, char* argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("2D transformation Tree");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard); 
    glutMainLoop();
    return 0;

}