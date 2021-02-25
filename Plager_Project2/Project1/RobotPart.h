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
		std::vector<float> trueVertices; 
		float rotation; 
		float joint[2]; 
		std::vector<RobotPart*> children; 
		RobotPart* parent = nullptr;
		int selectedChild = 0; 

		void doParentTransformations();
	public: 
		RobotPart(int id, float width, float height, float centerX, float centerY);

		float getRotation(); 
		std::vector<RobotPart*> getChildren(); 
		void setRotation(float); 
		void setJoint(float, float); 
		void setParent(RobotPart*); 

		void addChild(RobotPart*); 

		void select(); 
		void deselect(); 
		void draw(); 

		int selectDown(); 
		int selectUp();
		int selectRight(); 
		int selectLeft(); 

		void rotateCounterClockwise(); 
		void rotateClockwise();
};

