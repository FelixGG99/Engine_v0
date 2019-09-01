#ifndef INCS_H
#define INCS_H

/*TO-DO:

	Build assimp for x64 architecture
	Optimization
*/


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/glm.hpp>
#include<math.h>
#include<algorithm>
#include<bitset>
#include<vector>
#include<array>



#include "vec.h"   //first include vec.h since a lot of the following are dependant of it
#include "Shader.h"
#include "processInput.h"
#include "vertexdata.h"
#include "Projection.h"
#include "mesh.h"
#include "model.h"


#endif