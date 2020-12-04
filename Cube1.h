#pragma once
#include "Models.h"
float cube_vertices[] = {
	 -0.5f, -0.5f, -0.5f ,
	 -0.5f, 0.5f, -0.5f ,
	 0.5f, 0.5f, -0.5f ,
	 0.5f, -0.5f, -0.5f ,
	 -0.5f, -0.5f, 0.5f ,
	 -0.5f, 0.5f, 0.5f ,
	 0.5f, 0.5f, 0.5f ,
	 0.5f, -0.5f, 0.5f 
};
/*Vertex cube_vertices[] = {
	{ -0.5f, -0.5f, -0.5f },
	{ -0.5f, 0.5f, -0.5f },
	{ 0.5f, 0.5f, -0.5f },
	{ 0.5f, -0.5f, -0.5f },
	{ -0.5f, -0.5f, 0.5f },
	{ -0.5f, 0.5f, 0.5f },
	{ 0.5f, 0.5f, 0.5f },
	{ 0.5f, -0.5f, 0.5f }
};*/

GLuint cube_indices[] = {
	0,3,2,1,
	//0,1,2,3,
	4,5,6,7,
	0,1,5,4,
	1,2,6,5,
	2,3,7,6,
	3,0,4,7
};
float cube_normal[] = {
	 -0.5f, -0.5f, -0.5f ,
	 -0.5f, 0.5f, -0.5f ,
	 0.5f, 0.5f, -0.5f ,
	 0.5f, -0.5f, -0.5f ,
	 -0.5f, -0.5f, 0.5f ,
	 -0.5f, 0.5f, 0.5f ,
	 0.5f, 0.5f, 0.5f ,
	 0.5f, -0.5f, 0.5f
};