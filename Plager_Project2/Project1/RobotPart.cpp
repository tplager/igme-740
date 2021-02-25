#include "RobotPart.h"

/// <summary>
/// Constructor to create and set up a RobotPart
/// </summary>
/// <param name="id">The body part id of this robot part. There should be only 1 of each id</param>
/// <param name="width">The width of the rect that represents this robot part</param>
/// <param name="height">The height of the rect that represents this robot part</param>
/// <param name="centerX">The x position of the center of the rect that represents this robot part</param>
/// <param name="centerY">The y position of the center of the rect that represents this robot part</param>
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

/// <summary>
/// Adds a child to this robot part's children vector
/// </summary>
/// <param name="newChild">A pointer to another robot part that should be a child of this one</param>
void RobotPart::addChild(RobotPart* newChild)
{
	children.push_back(newChild); 

	newChild->parent = this;
}

/// <summary>
/// Selects this robot part
/// </summary>
void RobotPart::select()
{
	color[0] = 1.0f; 
	color[1] = 0.0f; 
	color[2] = 0.0f; 

	lineWidth = 4.0f;
}

/// <summary>
/// Deselects this robot part
/// </summary>
void RobotPart::deselect() 
{
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;

	lineWidth = 2.0f;
}

/// <summary>
/// Draws this robot part
/// </summary>
void RobotPart::draw()
{
	// Sets the Polygon Mode so that the RobotPart is drawn unfilled
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

	// Pushes the current transform matrix so that it can be returned to afterward
	glPushMatrix(); 

	// Recursively performs all of the parents' transformations, ending by
	// performing this RobotPart's own transformations
	doParentTransformations(); 

	// Draw according to Mesh's draw function
	Mesh::draw(); 

	// Pop the modified transform matrix off
	// Returning to the previous transform matrix
	glPopMatrix();

	// Resetting the Polygon Mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
}

/// <summary>
/// Selects a child of this RobotPart
/// </summary>
/// <returns>Returns the ID of that child or -1 if it fails</returns>
int RobotPart::selectDown()
{
	if (children.size() == 0)
		return -1; 

	children[selectedChild]->select(); 
	deselect(); 

	return children[selectedChild]->bodyPartID;
}

/// <summary>
/// Selects the parent of this RobotPart
/// </summary>
/// <returns>Returns the ID of the parent or -1 if it fails</returns>
int RobotPart::selectUp()
{
	if (parent == nullptr)
		return -1;

	parent->select(); 
	deselect(); 

	return parent->bodyPartID;
}

/// <summary>
/// Selects the next child of this RobotPart's parent
/// </summary>
/// <returns>Returns that RobotPart's ID or -1 if it fails</returns>
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

/// <summary>
/// Selects the previous child of this RobotPart's parent
/// </summary>
/// <returns>Returns that RobotPart's ID or -1 if it fails</returns>
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

/// <summary>
/// Increments the rotation value so that it rotates further counter clockwise on the next draw
/// </summary>
void RobotPart::rotateCounterClockwise()
{
	rotation += 2; 

	if (rotation >= 360)
		rotation -= 360;
}

/// <summary>
/// Decrements the rotation value so that it rotates further clockwise on the next draw
/// </summary>
void RobotPart::rotateClockwise()
{
	rotation -= 2;

	if (rotation <= 0)
		rotation += 360;
}

/// <summary>
/// Recursively performs the parents' transformations, ending by performing this RobotPart's own transformations
/// </summary>
void RobotPart::doParentTransformations()
{
	// If the parent is not null
	// Call this method on the parent RobotPart
	if (parent != nullptr) {
		parent->doParentTransformations(); 
	}

	// Translate to the joint's location and rotate the object by the rotation value
	glTranslatef(joint[0], joint[1], 0.0f); 
	glRotatef(rotation, 0.0f, 0.0f, 1.0f); 
}
