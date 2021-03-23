#include "PhysicalLight.h"
#include <GL/freeglut_std.h>

void PhysicalLight::draw()
{
	glUseProgram(0); 
	glPushMatrix();
	glColor3f(1.0f, 0, 0);
	glTranslatef(pos.x, pos.y, pos.z); 

	if (selected)
		glutSolidSphere(0.25f, 16, 16);
	else
		glutWireSphere(0.25f, 16, 16);
	
	glColor3f(1.0f, 1.0f, 1.0f);

	glPopMatrix();
}

void PhysicalLight::moveLight(vec3 addedPos)
{
	pos += addedPos; 
}

vec3 PhysicalLight::getPos()
{
	return pos; 
}

void PhysicalLight::setPos(vec3 newPos)
{
	pos = newPos; 
}

void PhysicalLight::select()
{
	selected = true;
}

void PhysicalLight::deselect()
{
	selected = false;
}
