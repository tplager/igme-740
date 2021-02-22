#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <vector>

class Mesh
{
	protected:
		std::vector<float> vertices; 
		float numVertices; 

		float* color; 
		float pointSize;
		float lineWidth; 
	public:
		Mesh();
		Mesh(float*, float, float);

		~Mesh(); 

		std::vector<float> getVertices();
		float getNumVertices(); 
		float* getColor();
		float getPointSize();
		float getLineWidth();

		void setMaxVertices(float);
		void setColor(float, float, float);
		void setPointSize(float);
		void setLineWidth(float);

		void virtual draw(const GLfloat*);
};

