
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <vector>
using namespace std;

float canvasSize[] = { 10.0f, 10.0f };
int rasterSize[] = { 800, 600 };

// structure for storing 3 2D vertices of a triangle
int numOfVertices = 0;
vector<float> v;
float color[3];

float mousePos[2];

int maxVertices = 1; 

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
    color[0] = 1.0f;
    color[1] = color[2] = 0.0f;
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

    if (numOfVertices > 0 && numOfVertices < maxVertices) {

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < numOfVertices; i++)
            glVertex2fv(&v.front() + i * 2);
        glVertex2fv(mousePos);
        glEnd();
    }
    else if (numOfVertices == maxVertices) {
        if (maxVertices > 2) {
            glBegin(GL_POLYGON);
        }
        else if (maxVertices == 1) {
            glBegin(GL_POINT); 
        }
        else if (maxVertices == 2) {
            glBegin(GL_LINE);
        }

        for (int i = 0; i < numOfVertices; i++) {
            glVertex2fv(&v.front() + i * 2);
        }

        glEnd();
    }

    drawCursor();
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
        if (numOfVertices >= maxVertices) {
            numOfVertices = 0;
            v.clear(); 
            
            if (maxVertices > 4) {
                maxVertices = INT_MAX; 
            }
        }

        mousePos[0] = (float)x / rasterSize[0] * canvasSize[0];
        mousePos[1] = (float)(rasterSize[1] - y) / rasterSize[1] * canvasSize[1];
        v.push_back(mousePos[0]);
        v.push_back(mousePos[1]);

        numOfVertices++;
        glutPostRedisplay();
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
            if (maxVertices == INT_MAX) {
                maxVertices = numOfVertices;
            }
            else {
                for (int i = v.size() / 2; i <= maxVertices; i++) { // This isn't right yet. This should probably push the last  
                    v.push_back(mousePos[0]);                       // vertex to the vector rather than the current mouse position
                    v.push_back(mousePos[1]);

                    numOfVertices++;
                }
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
        numOfVertices = 0;
        v.clear(); 

        if (maxVertices > 4) {
            maxVertices = INT_MAX;
        }

        glutPostRedisplay();
        break;
    case 1: //exit
        exit(0);
        break;
    case 2:                         // Set object to Point
        maxVertices = 1; 
        break;
    case 3:                         // Set object to Line
        maxVertices = 2; 
        break;
    case 4:                         // Set object to Triangle
        maxVertices = 3; 
        break;
    case 5:                         // Set object to Quad
        maxVertices = 4; 
        break;
    case 6:                         // Set object to Polygon
        maxVertices = INT_MAX;
        break;
    case 7:                         // Set color to Red
        color[0] = 1.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        glutPostRedisplay();
        break;
    case 8:                         // Set color to Green
        color[0] = 0.0f;
        color[1] = 1.0f;
        color[2] = 0.0f;
        glutPostRedisplay();
        break;
    case 9:                         // Set color to Blue
        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 1.0f;
        glutPostRedisplay();
        break;
    case 10:                        // Set color to Black
        color[0] = 0.0f; 
        color[1] = 0.0f; 
        color[2] = 0.0f; 
        glutPostRedisplay(); 
        break; 
    case 11:                        // Set point size to Small
        break;                              
    case 12:                        // Set point size to Medium
        break;                              
    case 13:                        // Set point size to Large
        break;
    case 14:                        // Set line width to Thin
        break;                             
    case 15:                        // Set line width to Average
        break;                             
    case 16:                        // Set line width to Thick
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