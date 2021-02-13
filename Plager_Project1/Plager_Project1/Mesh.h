#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <vector>

class Mesh
{
	private:
		std::vector<float> vertices; 
		float numVertices; 
		float maxVertices;

		float* color; 
		float pointSize;
		float lineWidth; 
		bool finished;
	public:
		Mesh();
		Mesh(float, float*, float, float);

		~Mesh(); 

		std::vector<float> getVertices();
		float getNumVertices(); 
		float getMaxVertices(); 
		float* getColor();
		float getPointSize();
		float getLineWidth();

		void setMaxVertices(float);
		void setColor(float, float, float);
		void setPointSize(float);
		void setLineWidth(float);
		void setFinished(bool);

		void draw(const GLfloat*);
		void addVertex(float, float); 
};

