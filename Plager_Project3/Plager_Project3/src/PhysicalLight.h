#pragma once
#include "Mesh.h"

class PhysicalLight
{
	private: 
		//Mesh object; 
		bool selected; 
		vec3 pos; 
	public: 
		void draw();

		void moveLight(vec3 addedPos); 

		vec3 getPos(); 
		void setPos(vec3 newPos); 

		void select(); 
		void deselect(); 
};

