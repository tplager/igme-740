#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "Mesh.h"

class RobotPart : public Mesh
{
	private: 
		int bodyPartID; 
		float rotation; 
		std::vector<RobotPart*> children; 
		RobotPart* parent = nullptr;
		int selectedChild = 0; 
	public: 
		RobotPart(int id, float height, float width, float centerX, float centerY);

		float getRotation(); 
		std::vector<RobotPart*> getChildren(); 
		void setRotation(float); 
		void setParent(RobotPart*); 

		void addChild(RobotPart*); 

		void select(); 
		void deselect(); 
		void draw(); 

		int selectDown(); 
		int selectUp();
		int selectRight(); 
		int selectLeft(); 
};

