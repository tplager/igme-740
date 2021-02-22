#include "Mesh.h"

Mesh::Mesh()
{
    numVertices = 0;

    color = new float[3];

    pointSize = 2.0f;
    lineWidth = 1.0f;
}

Mesh::Mesh(float* c, float pS, float lW)
{
    numVertices = 0;

    color = new float[3];
    color[0] = *c;
    color[1] = *(c + 1); 
    color[2] = *(c + 2);

    pointSize = pS; 
    lineWidth = lW; 
}

Mesh::~Mesh()
{
    delete color;
}

std::vector<float> Mesh::getVertices()
{
	return vertices; 
}

float Mesh::getNumVertices()
{
	return numVertices;
}

float* Mesh::getColor()
{
    return color;
}

float Mesh::getPointSize()
{
    return pointSize;
}

float Mesh::getLineWidth()
{
    return lineWidth;
}

void Mesh::setColor(float r, float g, float b)
{
    color[0] = r; 
    color[1] = g; 
    color[2] = b; 
}

void Mesh::setPointSize(float ps)
{
    pointSize = ps;
}

void Mesh::setLineWidth(float lw)
{
    lineWidth = lw; 
}

void Mesh::draw(const GLfloat* mousePos)
{
    glColor3fv(color);
    glLineWidth(lineWidth);

    if (numVertices > 0) {

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < numVertices; i++)
            glVertex2fv(&vertices.front() + i * 2);
        glVertex2fv(mousePos);
        glEnd();
    }

    glPointSize(pointSize);
    glBegin(GL_POINTS);
    for (int i = 0; i < numVertices; i++) {
        glVertex2fv(&vertices.front() + i * 2);
    }
    glEnd();
}
