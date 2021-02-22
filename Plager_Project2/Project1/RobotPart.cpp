#include "RobotPart.h"

RobotPart::RobotPart()
{
	color = new float[];
	Deselect(); 
}

void RobotPart::Select()
{
	color[0] = 1.0f; 
	color[1] = 0.0f; 
	color[2] = 0.0f; 
}

void RobotPart::Deselect() 
{
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
}
