#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <vector>
#include "RobotPart.h"

class Robot
{		
	private:
		std::vector<RobotPart> bodyParts; 
	public: 		
		Robot();
		void Draw(); 
};

