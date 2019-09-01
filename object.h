#ifndef OBJECT_H
#define OBJECT_H
#include "vec.h"
#include<vector>
#include<bitset>
class gObject {
	short int id, type;
	
public:
	class prop {
	public:
		const short int getX();
		const short int getY();
		const short int getZ();

		const short int isExistent();
		const short int isVisible();
		const short int isCollisionable();
		const short int isContainer();
		std::bitset<4>flags; //existent,visible,collisionable,container

	private:	
		std::vector<vec3>vertices;
		vec3 centerCoord;

	};

};

#endif