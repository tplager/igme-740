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

/// <summary>
/// Initializes the positions of the body parts of the robot
/// </summary>
void Robot::initBodyParts()
{
	bodyParts.push_back(RobotPart(BodyPartIndex::LowerBody, 2.2f, 1.6f, 0.0f, 0.8f));

	bodyParts.push_back(RobotPart(BodyPartIndex::UpperBody, 3.0f, 1.7f, 0.0f, 0.85f));

	bodyParts.push_back(RobotPart(BodyPartIndex::Neck, 0.6f, 1.0f, 0.0f, 0.5f));

	bodyParts.push_back(RobotPart(BodyPartIndex::Head, 1.8f, 1.8f, 0.0f, 0.9f));

	bodyParts.push_back(RobotPart(BodyPartIndex::RightArm, 1.5f, 0.7f, 0.75f, 0.0f));

	bodyParts.push_back(RobotPart(BodyPartIndex::RightForearm, 1.5f, 0.7f, 0.75f, 0.0));

	bodyParts.push_back(RobotPart(BodyPartIndex::RightHand, 1.1f, 1.1f, 0.55f, 0.0));

	bodyParts.push_back(RobotPart(BodyPartIndex::LeftArm, 1.5f, 0.7f, -0.75f, 0.0f));

	bodyParts.push_back(RobotPart(BodyPartIndex::LeftForearm, 1.5f, 0.7f, -0.75f, 0.0f));

	bodyParts.push_back(RobotPart(BodyPartIndex::LeftHand, 1.1f, 1.1f, -0.55f, 0.0f));

	bodyParts.push_back(RobotPart(BodyPartIndex::RightThigh, 0.7f, 1.8f, 0.0f, -0.9f));

	bodyParts.push_back(RobotPart(BodyPartIndex::RightLeg, 0.7f, 1.8f, 0.0f, -0.9f));

	bodyParts.push_back(RobotPart(BodyPartIndex::RightFoot, 1.6f, 0.8f, 0.4f, -0.4f));

	bodyParts.push_back(RobotPart(BodyPartIndex::LeftThigh, 0.7f, 1.8f, 0.0f, -0.9f));

	bodyParts.push_back(RobotPart(BodyPartIndex::LeftLeg, 0.7f, 1.8f, 0.0f, -0.9f));

	bodyParts.push_back(RobotPart(BodyPartIndex::LeftFoot, 1.6f, 0.8f, -0.4f, -0.4f));
}

/// <summary>
/// Initializes the relationships between the robot's body parts
/// </summary>
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

/// <summary>
/// Initializes the joints of each robot body part
/// </summary>
void Robot::initJoints()
{
	bodyParts[BodyPartIndex::LowerBody].setJoint(0.0, 0.0f);

	bodyParts[BodyPartIndex::UpperBody].setJoint(0.0f, 1.6f);

	bodyParts[BodyPartIndex::Neck].setJoint(0.0f, 1.7f);

	bodyParts[BodyPartIndex::Head].setJoint(0.0f, 1.0f);

	bodyParts[BodyPartIndex::LeftArm].setJoint(-1.5f, 0.8f);

	bodyParts[BodyPartIndex::LeftForearm].setJoint(-1.5f, 0.0f);

	bodyParts[BodyPartIndex::LeftHand].setJoint(-1.5f, 0.0f);

	bodyParts[BodyPartIndex::RightArm].setJoint(1.5f, 0.8f);

	bodyParts[BodyPartIndex::RightForearm].setJoint(1.5f, 0.0f);

	bodyParts[BodyPartIndex::RightHand].setJoint(1.5f, 0.0f);

	bodyParts[BodyPartIndex::LeftThigh].setJoint(-0.75, 0.0f);

	bodyParts[BodyPartIndex::LeftLeg].setJoint(0.0, -1.8f);

	bodyParts[BodyPartIndex::LeftFoot].setJoint(0.05, -1.8f);

	bodyParts[BodyPartIndex::RightThigh].setJoint(0.75f, 0.0f);

	bodyParts[BodyPartIndex::RightLeg].setJoint(0.0f, -1.8f);

	bodyParts[BodyPartIndex::RightFoot].setJoint(0.05f, -1.8f);
}

Robot::Robot()
{
	// Set Up Body Parts
	initBodyParts(); 
	initBodyPartRelations(); 
	initJoints();

	bodyParts[selectedIndex].select(); 
}

/// <summary>
/// Draws all the robot's body parts
/// </summary>
void Robot::draw()
{
	for (int i = 0; i < bodyParts.size(); i++) {
		bodyParts[i].draw(); 
	}
}

/// <summary>
/// Selects a child of the currently selected body part if possible
/// </summary>
void Robot::selectDown() 
{
	int temp = bodyParts[selectedIndex].selectDown(); 

	if (temp == -1)
		return;
	else
		selectedIndex = temp;
}

/// <summary>
/// Selects the parent of the currently selected body part if possible
/// </summary>
void Robot::selectUp()
{
	int temp = bodyParts[selectedIndex].selectUp();
	
	if (temp == -1)
		return;
	else
		selectedIndex = temp;
}

/// <summary>
/// Selects the next child of the currently selected body part's parent's children vector
/// </summary>
void Robot::selectRight()
{
	int temp = bodyParts[selectedIndex].selectRight();

	if (temp == -1)
		return;
	else
		selectedIndex = temp;
}

/// <summary>
/// Selects the previous child of the currently selected body part's parent's children vector
/// </summary>
void Robot::selectLeft()
{
	int temp = bodyParts[selectedIndex].selectLeft();

	if (temp == -1)
		return;
	else
		selectedIndex = temp;
}

/// <summary>
/// Rotates the currently selected body part counter clockwise
/// </summary>
void Robot::rotateCounterClockwise()
{
	bodyParts[selectedIndex].rotateCounterClockwise(); 
}

/// <summary>
/// Rotates the currently selected body part clockwise
/// </summary>
void Robot::rotateClockwise()
{
	bodyParts[selectedIndex].rotateClockwise(); 
}
