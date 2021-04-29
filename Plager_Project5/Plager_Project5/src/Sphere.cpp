#include "Sphere.h"

Sphere::Sphere()
{
    pos = vec3(0.0); 
    radius = 0; 
    color = vec3(1.0); 

    ambient = 1.0f; 
    diffuse = 1.0f; 
    phong = 1.0f; 
}

Sphere::Sphere(vec3 p, float r, vec3 c, float a, float d, float ph)
{
    pos = p; 
    radius = r; 
    color = c; 

    ambient = a; 
    diffuse = d; 
    phong = ph; 
}

void Sphere::Draw()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glColor3f(color.x, color.y, color.z);
    glutWireSphere(radius, 10, 10);

    glPopMatrix();
};