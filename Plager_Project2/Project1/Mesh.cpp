#include "Mesh.h"

Mesh::Mesh()
{
    numVertices = 0;

    color = new float[3];

    pointSize = 1.0f;
    lineWidth = 1.0f;
}

Mesh::Mesh(const Mesh& other)
{
    numVertices = other.numVertices; 

    vertices = other.vertices;

    color = new float[3];
    color[0] = other.color[0]; 
    color[1] = other.color[1];
    color[2] = other.color[2];

    pointSize = other.pointSize; 
    lineWidth = other.lineWidth;
}

void Mesh::operator=(const Mesh& other)
{
    numVertices = other.numVertices;

    vertices.clear(); 
    vertices = other.vertices;

    color[0] = other.color[0];
    color[1] = other.color[1];
    color[2] = other.color[2];

    pointSize = other.pointSize;
    lineWidth = other.lineWidth;
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
    delete[] color;
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

void Mesh::draw()
{
    glColor3fv(color);
    glLineWidth(lineWidth);

    glBegin(GL_POLYGON);

    for (int i = 0; i < numVertices; i++) {
        glVertex2fv(&vertices.front() + i * 2);
    }

    glEnd();

    glEnd();
}
