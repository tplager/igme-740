#include "RobotPart.h"

RobotPart::RobotPart(int id, float width, float height, float centerX, float centerY) : Mesh()
{
	bodyPartID = id;

	vertices.push_back(centerX - width/2);
	vertices.push_back(centerY + height/2);
	vertices.push_back(centerX + width / 2);
	vertices.push_back(centerY + height / 2);
	vertices.push_back(centerX + width / 2);
	vertices.push_back(centerY - height / 2);
	vertices.push_back(centerX - width / 2);
	vertices.push_back(centerY - height / 2);

	trueVertices.push_back(centerX - width / 2);
	trueVertices.push_back(centerY + height / 2);
	trueVertices.push_back(centerX + width / 2);
	trueVertices.push_back(centerY + height / 2);
	trueVertices.push_back(centerX + width / 2);
	trueVertices.push_back(centerY - height / 2);
	trueVertices.push_back(centerX - width / 2);
	trueVertices.push_back(centerY - height / 2);

	numVertices = 4; 

	lineWidth = 2.0f; 

	rotation = 0.0f; 

	joint[0] = 0.0f; 
	joint[1] = 0.0f; 
}

float RobotPart::getRotation()
{
	return rotation;
}

std::vector<RobotPart*> RobotPart::getChildren()
{
	return children;
}

void RobotPart::setRotation(float value)
{
	rotation = value;
}

void RobotPart::setJoint(float x, float y)
{
	joint[0] = x;
	joint[1] = y;
}

void RobotPart::setParent(RobotPart* value)
{
	parent = value; 
}

void RobotPart::addChild(RobotPart* newChild)
{
	children.push_back(newChild); 

	newChild->parent = this;
}

void RobotPart::select()
{
	color[0] = 1.0f; 
	color[1] = 0.0f; 
	color[2] = 0.0f; 

	lineWidth = 4.0f;
}

void RobotPart::deselect() 
{
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;

	lineWidth = 2.0f;
}

void RobotPart::draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

	glPushMatrix(); 

	doParentTransformations(); 

	Mesh::draw(); 

	glPopMatrix();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
}

int RobotPart::selectDown()
{
	if (children.size() == 0)
		return -1; 

	children[selectedChild]->select(); 
	deselect(); 

	return children[selectedChild]->bodyPartID;
}

int RobotPart::selectUp()
{
	if (parent == nullptr)
		return -1;

	parent->select(); 
	deselect(); 

	return parent->bodyPartID;
}

int RobotPart::selectRight()
{
	if (parent == nullptr || parent->children.size() == 1)
		return -1;

	parent->selectedChild++; 

	if (parent->selectedChild >= parent->children.size()) {
		parent->selectedChild = 0; 
	}

	deselect();

	return parent->selectDown();
}

int RobotPart::selectLeft()
{
	if (parent == nullptr || parent->children.size() == 1)
		return -1;

	parent->selectedChild--;

	if (parent->selectedChild < 0) {
		parent->selectedChild = parent->children.size() - 1;
	}

	deselect();

	return parent->selectDown();
}

void RobotPart::rotateCounterClockwise()
{
	rotation += 2; 
}

void RobotPart::rotateClockwise()
{
	rotation -= 2;
}

void RobotPart::doParentTransformations()
{
	if (parent != nullptr) {
		parent->doParentTransformations(); 
	}

	glTranslatef(joint[0], joint[1], 0.0f); 
	glRotatef(rotation, 0.0f, 0.0f, 1.0f); 
}
