#pragma once
#include "Models.h"
float floor_vertices[] = {
	-5.0f, 0.0f, -5.0f,//A
	-5.0f, 0.0f,  5.0f,//B
	 5.0f, 0.0f,  5.0f,//C
	 5.0f, 0.0f, -5.0f //D
	
};
GLuint floor_indices[] = {
	0,2,1,
	0,3,2
};
float floor_normals[] = {
	0.0f, 1.0f, 0.0f,//A
	0.0f, 1.0f, 0.0f,//B
	0.0f, 1.0f, 0.0f,//C
	0.0f, 1.0f, 0.0f,//D

};