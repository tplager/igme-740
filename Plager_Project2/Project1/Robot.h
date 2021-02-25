#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <vector>
#include <algorithm>

#include "RobotPart.h"

class Robot
{		
	private:
		std::vector<RobotPart> bodyParts; 
		int selectedIndex = 0; 
		void initBodyParts(); 
		void initBodyPartRelations(); 
		void initJoints(); 
	public: 		
		Robot();

		void draw(); 

		void selectDown();
		void selectUp();
		void selectRight();
		void selectLeft();

		void rotateCounterClockwise(); 
		void rotateClockwise(); 
};

