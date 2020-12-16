#pragma once
#include "Models.h"
float floor_vertices[] = {
	-5.0f, 0.0f, -5.0f,//A
	-5.0f, 0.0f,  5.0f,//B
	 5.0f, 0.0f,  5.0f,//C
	 5.0f, 0.0f, -5.0f //D
	
};
GLuint floor_indices[] = {
	0,3,2,1,//2,0,
};
float floor_normals[] = {
	0.0f, 1.0f, 0.0f,//A
	0.0f, 1.0f, 0.0f,//B
	0.0f, 1.0f, 0.0f,//C
	0.0f, 1.0f, 0.0f,//D

};

float floor_text_normal[] =
{
	1,1,
	1,0,	
	0,0,
	0,1,	
};