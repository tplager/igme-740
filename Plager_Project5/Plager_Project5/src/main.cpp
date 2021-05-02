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
#include "RecPrism.h"
#include "Light.h"

#include "Text.h"

#include <fstream>
#include <iostream>
using namespace std;
using namespace glm;

int g_winWidth  = 640;
int g_winHeight = 480;

Camera g_cam;
Text g_text;
unsigned char g_keyStates[256];

GLfloat light0_pos[] = {0.0f, 5.0f, 5.0f, 0.0f};
GLfloat light0_Amb[] = {0.4f, 0.3f, 0.3f, 1.0f};
GLfloat light0_Diff[] = {0.8f, 0.8f, 0.7f, 1.0f};
GLfloat light0_Spec[] = {0.9f, 0.9f, 0.9f, 1.0f};

const char dataFile[128] = "geoData/geo.txt";

char v_shader_file[] = ".\\shaders\\v_shader.vert";
char f_shader_file[] = ".\\shaders\\f_shader.frag";
const GLchar c_shader_file[] = ".\\shaders\\c_shader.comp";

ShaderClass cShader; 
ShaderProgram cShaderProg; 

unsigned char* imagedata;

float vertices[4 * 2] = 
{
    -320, -240, 
    320, -240, 
    320, 240, 
    -320, 240 
}; 

float texCoords[4 * 2] = 
{ 
    0, 0, 
    1, 0, 
    1, 1, 
    0, 1 
};

GLuint glTexID = -1;

GLuint pos_ssbo;
GLuint color_ssbo;
GLuint test_ssbo;

vec4* pos_array;
vec4* color_array;
char* test_array; 

unsigned int g_box_num;
RecPrism* g_boxes;

unsigned int g_sphere_num;
Sphere * g_spheres;

Light g_light;

void loadConfigFile(const char* pFilePath)
{
    const unsigned int CAMERA= 0;
    const unsigned int LIGHT= 1;
    const unsigned int SPHERE= 2;
    const unsigned int BOX = 3;
    
    vec3 rot;
    fstream filestr;
    filestr.open (pFilePath, fstream::in);
    if(!filestr.is_open()){
        cout<<"'"<<pFilePath<<"'"<<", geo file does not exsit!"<<endl;
        filestr.close();
        return;
    }
    
    char objType[80];
    char attrType[80];
    unsigned int objState = -1;
    
    bool loop = true;

    vec3 colorBuffer = vec3(1.0f);
    float ambientBuffer = 1.0f;
    float diffuseBuffer = 1.0f;
    float phongBuffer = 1.0f;

    while(loop){
        filestr>>objType;
        if(!strcmp(objType, "Camera:")) objState=CAMERA;
        else if(!strcmp(objType, "Light:")) objState = LIGHT;
        else if(!strcmp(objType, "Sphere:")) objState = SPHERE;
        else if(!strcmp(objType, "Box:")) objState = BOX;
        switch(objState){
            case CAMERA:
                float e_x, e_y, e_z;
                float l_x, l_y, l_z;
                float near_plane;
                
                for(int i=0; i<3; i++){
                    filestr>>attrType;
                    if(!strcmp(attrType, "eye:")){
                        filestr>>e_x;
                        filestr>>e_y;
                        filestr>>e_z;
                    }
                    else if(!strcmp(attrType, "lookat:")){
                        filestr>>l_x;
                        filestr>>l_y;
                        filestr>>l_z;
                    }
                    else if(!strcmp(attrType, "near_plane:")) filestr>>near_plane;
                }
                g_cam.set(e_x, e_y, e_z, l_x, l_y, l_z, g_winWidth, g_winHeight, 45.0f, near_plane, 1000.0f);
                break;
            case LIGHT:
                filestr>>attrType;
                if(!strcmp(attrType, "position:")){
                    filestr>>g_light.pos.x;
                    filestr>>g_light.pos.y;
                    filestr>>g_light.pos.z;
                }
                filestr>>attrType;
                if(!strcmp(attrType, "color:")){
                    filestr>>g_light.color.x;
                    filestr>>g_light.color.y;
                    filestr>>g_light.color.z;
                }
                filestr>>attrType;
                if(!strcmp(attrType, "intensity:")){
                    filestr>>g_light.intensity;
                }
                break;
            case SPHERE: 
            {
                vec3 posBuffer = vec3(0.0f);
                float radiusBuffer = 1.0f;


                filestr >> attrType;
                if (!strcmp(attrType, "num:"))
                    filestr >> g_sphere_num;
                if (g_sphere_num > 0) {
                    g_spheres = new Sphere[g_sphere_num];

                    for (int i = 0; i < g_sphere_num; i++)
                    {
                        filestr >> attrType;
                        if (!strcmp(attrType, "position:")) {
                            filestr >> posBuffer.x;
                            filestr >> posBuffer.y;
                            filestr >> posBuffer.z;
                        }
                        filestr >> attrType;
                        if (!strcmp(attrType, "radius:")) {
                            filestr >> radiusBuffer;
                        }
                        filestr >> attrType;
                        if (!strcmp(attrType, "color:")) {
                            filestr >> colorBuffer.x;
                            filestr >> colorBuffer.y;
                            filestr >> colorBuffer.z;
                        }
                        filestr >> attrType;
                        if (!strcmp(attrType, "ambient:"))	filestr >> ambientBuffer;
                        filestr >> attrType;
                        if (!strcmp(attrType, "diffuse:"))	filestr >> diffuseBuffer;
                        filestr >> attrType;
                        if (!strcmp(attrType, "phong:")) 	filestr >> phongBuffer;

                        g_spheres[i] = Sphere(posBuffer, radiusBuffer, colorBuffer, ambientBuffer, diffuseBuffer, phongBuffer);
                    }
                }
                break;
            }
            case BOX:
            {
                vec3 minPosBuffer = vec3(0.0f);
                vec3 maxPosBuffer = vec3(0.0f);
                mat4 rotMatBuffer = mat4(0.0f);
                mat4 invRotMatBuffer = mat4(0.0f);

                filestr >> attrType;
                if (!strcmp(attrType, "num:"))
                    filestr >> g_box_num;

                if (g_box_num > 0) {
                    g_boxes = new RecPrism[g_box_num];
                    for (int i = 0; i < g_box_num; i++) {
                        filestr >> attrType;
                        if (!strcmp(attrType, "conner_position:")) {

                            filestr >> minPosBuffer.x;
                            filestr >> minPosBuffer.y;
                            filestr >> minPosBuffer.z;
                            filestr >> maxPosBuffer.x;
                            filestr >> maxPosBuffer.y;
                            filestr >> maxPosBuffer.z;
                        }

                        filestr >> attrType;
                        if (!strcmp(attrType, "color:")) {
                            filestr >> colorBuffer.x;
                            filestr >> colorBuffer.y;
                            filestr >> colorBuffer.z;
                        }
                        filestr >> attrType;
                        if (!strcmp(attrType, "rotate:")) {
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

                            rotMatBuffer = m;
                            invRotMatBuffer = inverse(m);
                        }

                        filestr >> attrType;
                        if (!strcmp(attrType, "ambient:"))	filestr >> ambientBuffer;
                        filestr >> attrType;
                        if (!strcmp(attrType, "diffuse:"))	filestr >> diffuseBuffer;
                        filestr >> attrType;
                        if (!strcmp(attrType, "phong:"))	filestr >> phongBuffer;

                        g_boxes[i].init(minPosBuffer, maxPosBuffer, colorBuffer, rotMatBuffer, invRotMatBuffer, ambientBuffer, diffuseBuffer, phongBuffer);
                        g_boxes[i].create(v_shader_file, f_shader_file, c_shader_file); 
                    }
                    loop = false;
                }
                break;
            }
        }
    }
}

void createTexture()
{
    imagedata = new unsigned char[g_winWidth * g_winHeight * 3];

    int color_index = 0;
    // assign black color (0, 0 , 0) to each pixel
    for (int i = 0; i < g_winWidth * g_winHeight; i++)
    {
        imagedata[i * 3 + 0] = color_array[color_index].r * 255; // R
        imagedata[i * 3 + 1] = color_array[color_index].g * 255;   // G
        imagedata[i * 3 + 2] = color_array[color_index].b * 255;   // B
    }

    glGenTextures(1, &glTexID);
    glBindTexture(GL_TEXTURE_2D, glTexID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // send the imagedata (on CPU) to the GPU memory at glTexID (glTexID is a GPU memory location index)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_winWidth, g_winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata);
}

void initialization() 
{    
    g_cam.set(3.0f, 4.0f, 3.0f, 0.0f, 0.0f, 0.0f, g_winWidth, g_winHeight);

    loadConfigFile(dataFile);

    // adding stuff to draw ...
    cShader.create(c_shader_file, GL_COMPUTE_SHADER);
    cShaderProg.create();
    cShaderProg.link(cShader); // link compute shader 
    cShader.destroy();
    
    //********* create and fulfill the particle data into shader storage buffer objects (for gen-purpose computing) **********
    GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

    // creaste a ssbo of particle positions
    glGenBuffers(1, &pos_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, pos_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, (640 * 480) * sizeof(vec4), NULL, GL_STATIC_DRAW); // there isn't data yet, just init memory, data will sent at run-time. 

    // map and create the postion array
    pos_array = (vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, (640 * 480) * sizeof(vec4), bufMask);

    vec2 size_max_point = vec2(320, 240); 
    vec2 size_min_point = vec2(-320, -240); 
    if (pos_array != NULL)
    {
        int i = 0;
        for (unsigned int x = 0; x < 640; x++)
        {
            for (unsigned int y = 0; y < 480; y++) {
                pos_array[i].x = (((size_max_point.x - size_min_point.x) / 640) * x) + size_min_point.x;
                pos_array[i].y = (((size_max_point.y - size_min_point.y) / 480) * y) + size_min_point.y;
                pos_array[i].z = -5.0f;
                pos_array[i].w = 1.0f;

                i++;
            }
        }
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    // map and create the color array
    glGenBuffers(1, &color_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, color_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, (640 * 480) * sizeof(vec4), NULL, GL_STATIC_DRAW);
    color_array = (vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, (640 * 480) * sizeof(vec4), bufMask);

    if (color_array != NULL)
    {
        for (unsigned int i = 0; i < (640 * 480); i++)
        {
            color_array[i].r = 1.0f;
            color_array[i].g = 1.0f;
            color_array[i].b = 1.0f;
            color_array[i].a = 1.0f;
        }
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    // bind the SSBOs to the labeled "binding" in the compute shader using assigned layout labels
    // this is similar to mapping data to attribute variables in the vertex shader
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, pos_ssbo);    // 4 - lay out id for positions in compute shader 
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, color_ssbo);	// 6 - lay out id for colors in compute shade

    createTexture();

	g_text.setColor(0.0f, 0.0f, 0.0f);
}

/****** GL callbacks ******/
void initialGL()
{    
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_Amb);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_Diff);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_Spec);


	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glPolygonMode(GL_FRONT, GL_FILL);

	
	glClearColor (1.0f, 1.0f, 1.0f, 0.0f);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

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

    glUseProgram(cShaderProg.id);
    glDispatchCompute((GLuint)g_winWidth, (GLuint)g_winHeight, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, (640 * 480) * sizeof(vec4), color_array);

    createTexture();

    drawPlane();

    //// draw sphere and box
    //for (int i=0; i<g_sphere_num; i++)
    //    g_spheres[i].draw();
    //for (int i=0; i<g_box_num; i++)
    //    g_boxes[i].draw(g_cam);

    glutSwapBuffers();
}

void reshape(int w, int h)
{
	g_winWidth = w;
	g_winHeight = h;
	if (h == 0) {
		h = 1;
	}
	g_cam.setProj(g_winWidth, g_winHeight);
    g_cam.setModelView();
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

void keyboard(unsigned char key, int x, int y)
{
    g_keyStates[key] = true;
	switch(key) { 
		case 27:
			exit(0);
			break;
        case 'c': // switch cam control mode
            g_cam.switchCamMode();
			glutPostRedisplay();
            break;
        case ' ':
            g_cam.PrintProperty();
            break;

	}
}


int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(g_winWidth, g_winHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Ray Casting");
	
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