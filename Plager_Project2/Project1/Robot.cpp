#include "Robot.h"

enum BodyPartIndex {
	LowerBody, 
	UpperBody, 
	Neck, 
	Head, 
	RightArm, 
	RightForearm, 
	RightHand, 
	LeftArm, 
	LeftForearm, 
	LeftHand, 
	RightThigh, 
	RightLeg, 
	RightFoot, 
	LeftThigh, 
	LeftLeg, 
	LeftFoot
};
void Robot::initBodyParts()
{
	bodyParts.push_back(RobotPart(BodyPartIndex::LowerBody, 2.2f, 1.6f, 0.0f, 0.0f));

	bodyParts.push_back(RobotPart(BodyPartIndex::UpperBody, 3.0f, 1.7f, 0.0f, 1.65f));

	bodyParts.push_back(RobotPart(BodyPartIndex::Neck, 0.6f, 1.0f, 0.0f, 3.0f));

	bodyParts.push_back(RobotPart(BodyPartIndex::Head, 1.8f, 1.8f, 0.0f, 4.4f));

	bodyParts.push_back(RobotPart(BodyPartIndex::RightArm, 1.5f, 0.7f, 2.25f, 1.65f));

	bodyParts.push_back(RobotPart(BodyPartIndex::RightForearm, 1.5f, 0.7f, 3.75f, 1.65f));

	bodyParts.push_back(RobotPart(BodyPartIndex::RightHand, 1.1f, 1.1f, 5.05f, 1.65f));

	bodyParts.push_back(RobotPart(BodyPartIndex::LeftArm, 1.5f, 0.7f, -2.25f, 1.65f));

	bodyParts.push_back(RobotPart(BodyPartIndex::LeftForearm, 1.5f, 0.7f, -3.75f, 1.65f));

	bodyParts.push_back(RobotPart(BodyPartIndex::LeftHand, 1.1f, 1.1f, -5.05f, 1.65f));

	bodyParts.push_back(RobotPart(BodyPartIndex::RightThigh, 0.7f, 1.8f, 0.75f, -1.7f));

	bodyParts.push_back(RobotPart(BodyPartIndex::RightLeg, 0.7f, 1.8f, 0.75f, -3.5f));

	bodyParts.push_back(RobotPart(BodyPartIndex::RightFoot, 1.6f, 0.8f, 1.2f, -4.8f));

	bodyParts.push_back(RobotPart(BodyPartIndex::LeftThigh, 0.7f, 1.8f, -0.75f, -1.7f));

	bodyParts.push_back(RobotPart(BodyPartIndex::LeftLeg, 0.7f, 1.8f, -0.75f, -3.5f));

	bodyParts.push_back(RobotPart(BodyPartIndex::LeftFoot, 1.6f, 0.8f, -1.2f, -4.8f));
}

void Robot::initBodyPartRelations()
{
	bodyParts[BodyPartIndex::LowerBody].addChild(&bodyParts[BodyPartIndex::UpperBody]); 
	bodyParts[BodyPartIndex::LowerBody].addChild(&bodyParts[BodyPartIndex::LeftThigh]);
	bodyParts[BodyPartIndex::LowerBody].addChild(&bodyParts[BodyPartIndex::RightThigh]);

	bodyParts[BodyPartIndex::UpperBody].addChild(&bodyParts[BodyPartIndex::Neck]); 
	bodyParts[BodyPartIndex::UpperBody].addChild(&bodyParts[BodyPartIndex::LeftArm]); 
	bodyParts[BodyPartIndex::UpperBody].addChild(&bodyParts[BodyPartIndex::RightArm]); 

	bodyParts[BodyPartIndex::Neck].addChild(&bodyParts[BodyPartIndex::Head]); 

	bodyParts[BodyPartIndex::LeftArm].addChild(&bodyParts[BodyPartIndex::LeftForearm]); 

	bodyParts[BodyPartIndex::LeftForearm].addChild(&bodyParts[BodyPartIndex::LeftHand]);

	bodyParts[BodyPartIndex::RightArm].addChild(&bodyParts[BodyPartIndex::RightForearm]);

	bodyParts[BodyPartIndex::RightForearm].addChild(&bodyParts[BodyPartIndex::RightHand]);

	bodyParts[BodyPartIndex::LeftThigh].addChild(&bodyParts[BodyPartIndex::LeftLeg]); 

	bodyParts[BodyPartIndex::LeftLeg].addChild(&bodyParts[BodyPartIndex::LeftFoot]); 

	bodyParts[BodyPartIndex::RightThigh].addChild(&bodyParts[BodyPartIndex::RightLeg]);

	bodyParts[BodyPartIndex::RightLeg].addChild(&bodyParts[BodyPartIndex::RightFoot]);
}

Robot::Robot()
{
	// Set Up Body Parts
	initBodyParts(); 
	initBodyPartRelations(); 

	bodyParts[selectedIndex].select(); 

	//testing
	//bodyParts[0].setRotation(45.0f); 
}

void Robot::draw()
{
	for (int i = 0; i < bodyParts.size(); i++) {
		bodyParts[i].draw(); 
	}
}

void Robot::selectDown() 
{
	int temp = bodyParts[selectedIndex].selectDown(); 

	if (temp == -1)
		return;
	else
		selectedIndex = temp;
}

void Robot::selectUp()
{
	int temp = bodyParts[selectedIndex].selectUp();
	
	if (temp == -1)
		return;
	else
		selectedIndex = temp;
}

void Robot::selectRight()
{
	int temp = bodyParts[selectedIndex].selectRight();

	if (temp == -1)
		return;
	else
		selectedIndex = temp;
}

void Robot::selectLeft()
{
	int temp = bodyParts[selectedIndex].selectLeft();

	if (temp == -1)
		return;
	else
		selectedIndex = temp;
}
