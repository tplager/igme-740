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

	numVertices = 4; 

	lineWidth = 2.0f; 

	rotation = 0.0f; 
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

void RobotPart::setParent(RobotPart* value)
{
	parent = value; 
}

void RobotPart::addChild(RobotPart* newChild)
{
	children.push_back(newChild); 
}

void RobotPart::select()
{
	color[0] = 1.0f; 
	color[1] = 0.0f; 
	color[2] = 0.0f; 

	lineWidth = 8.0f;
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

	glRotatef(rotation, 0.0f, 0.0f, 1.0f); 

	Mesh::draw(); 

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
}

int RobotPart::selectDown()
{
	if (children.size() == 0)
		return -1; 

	children[selectedChild]->setParent(this);
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
