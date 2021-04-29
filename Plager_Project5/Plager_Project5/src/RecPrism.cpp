#include "RecPrism.h"

RecPrism::RecPrism()
{
    minPos = vec3(0.0);
    maxPos = vec3(1.0);
    color = vec3(1.0);

    mat4 rotMat = mat4(0.0f); 
    mat4 invRotMat = mat4(0.0f); 

    ambient = 1.0f;
    diffuse = 1.0f;
    phong = 1.0f;
}

RecPrism::RecPrism(vec3 minP, vec3 maxP, vec3 c, mat4 rM, mat4 iRM, float a, float d, float p)
{
    minPos = minP; 
    maxPos = maxP; 
    color = c; 

    rotMat = rM; 
    invRotMat = iRM; 

    ambient = a; 
    diffuse = d; 
    phong = p;
}

vec3 RecPrism::getMinPos()
{
    return minPos; 
}

vec3 RecPrism::getMaxPos()
{
    return maxPos;
}

vec3 RecPrism::getColor()
{
    return color; 
}

void RecPrism::Draw()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);


    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf((const float*)glm::value_ptr(rotMat));

    glColor3f(color.x, color.y, color.z);

    glBegin(GL_QUADS);

    // Top face (y = 1.0f)
    // Counter-clockwise (CCW) with normal pointing out
    glVertex3f(maxPos.x, maxPos.y, minPos.z);
    glVertex3f(minPos.x, maxPos.y, minPos.z);
    glVertex3f(minPos.x, maxPos.y, maxPos.z);
    glVertex3f(maxPos.x, maxPos.y, maxPos.z);

    // Bottom face (y = -1.0f)
    glVertex3f(maxPos.x, minPos.y, maxPos.z);
    glVertex3f(minPos.x, minPos.y, maxPos.z);
    glVertex3f(minPos.x, minPos.y, minPos.z);
    glVertex3f(maxPos.x, minPos.y, minPos.z);

    // Front face  (z = 1.0f)
    glVertex3f(maxPos.x, maxPos.y, maxPos.z);
    glVertex3f(minPos.x, maxPos.y, maxPos.z);
    glVertex3f(minPos.x, minPos.y, maxPos.z);
    glVertex3f(maxPos.x, minPos.y, maxPos.z);

    // Back face (z = -1.0f)
    glVertex3f(maxPos.x, minPos.y, minPos.z);
    glVertex3f(minPos.x, minPos.y, minPos.z);
    glVertex3f(minPos.x, maxPos.y, minPos.z);
    glVertex3f(maxPos.x, maxPos.y, minPos.z);

    // Left face (x = -1.0f)
    glVertex3f(minPos.x, maxPos.y, maxPos.z);
    glVertex3f(minPos.x, maxPos.y, minPos.z);
    glVertex3f(minPos.x, minPos.y, minPos.z);
    glVertex3f(minPos.x, minPos.y, maxPos.z);

    // Right face (x = 1.0f)
    glVertex3f(maxPos.x, maxPos.y, minPos.z);
    glVertex3f(maxPos.x, maxPos.y, maxPos.z);
    glVertex3f(maxPos.x, minPos.y, maxPos.z);
    glVertex3f(maxPos.x, minPos.y, minPos.z);
    glEnd();

    glPopMatrix();
}
